<#
.SYNOPSIS
    Scans your NeetCode 150 submissions repo and auto-updates the README stats.

.DESCRIPTION
    - Walks folders listed in problems.json -> sourceFolders
    - Any subfolder containing a "submission-*" file counts as SOLVED
    - Matches folder names (slugs) against problems.json for difficulty + topic
    - Rewrites the AUTO:STATS and AUTO:ROADMAP blocks in README.md
    - Prints unmatched folders so you can fix slug mappings

.PARAMETER RepoRoot
    Root of the repo. Defaults to the script's directory.

.PARAMETER ReadmePath
    Path to README.md. Defaults to <RepoRoot>\README.md.

.PARAMETER ProblemsPath
    Path to problems.json. Defaults to <RepoRoot>\problems.json.

.PARAMETER UseGit
    If set, uses `git log` to get accurate first-commit dates for streak math
    (more reliable than file mtimes after a fresh clone).

.EXAMPLE
    .\Update-Stats.ps1
    .\Update-Stats.ps1 -UseGit
    .\Update-Stats.ps1 -RepoRoot 'C:\code\neetcode-submissions' -UseGit
#>

[CmdletBinding()]
param(
    [string]$RepoRoot = $PSScriptRoot,
    [string]$ReadmePath,
    [string]$ProblemsPath,
    [switch]$UseGit
)

$ErrorActionPreference = 'Stop'

if (-not $ReadmePath)   { $ReadmePath   = Join-Path $RepoRoot 'README.md' }
if (-not $ProblemsPath) { $ProblemsPath = Join-Path $RepoRoot 'problems.json' }

if (-not (Test-Path $ReadmePath))   { throw "README not found: $ReadmePath" }
if (-not (Test-Path $ProblemsPath)) { throw "problems.json not found: $ProblemsPath" }

Write-Host "-> Loading catalog: $ProblemsPath" -ForegroundColor Cyan
$data = Get-Content $ProblemsPath -Raw | ConvertFrom-Json

# ---------------------------------------------------------------- lookup
$slugMap     = @{}   # lowercase-slug -> [topic, difficulty, title]
$topicCounts = [ordered]@{}
$topicSolved = [ordered]@{}
foreach ($t in $data.topics) {
    $topicCounts[$t.name] = $t.problems.Count
    $topicSolved[$t.name] = 0
    foreach ($p in $t.problems) {
        $slugMap[$p.slug.ToLowerInvariant()] = [PSCustomObject]@{
            Topic = $t.name; Difficulty = $p.difficulty; Title = $p.title; Slug = $p.slug
        }
    }
}

# ---------------------------------------------------------------- fuzzy matching
# NeetCode's auto-sync uses its own slugified titles (e.g. "invert-a-binary-tree"),
# which don't always match LeetCode slugs in problems.json. We tolerate that with
# an IDF-weighted token-set Jaccard match when the exact key lookup fails.
# IDF weighting is critical: without it, "binary" + "tree" (common tokens) cause
# ties between e.g. "subtree-of-another-tree" and "balanced-binary-tree".
$stopwords = @{ 'a'=1; 'an'=1; 'the'=1; 'of'=1; 'in'=1; 'to'=1; 'for'=1; 'and'=1; 'on'=1; 'with'=1 }

function Get-SlugTokens([string]$slug) {
    $parts = $slug.ToLowerInvariant() -split '-'
    return @($parts | Where-Object { $_ -and -not $stopwords.ContainsKey($_) } | Sort-Object -Unique)
}

$slugTokens = @{}
$docFreq    = @{}
foreach ($k in $slugMap.Keys) {
    $toks = Get-SlugTokens $k
    $slugTokens[$k] = $toks
    foreach ($t in $toks) {
        if ($docFreq.ContainsKey($t)) { $docFreq[$t]++ } else { $docFreq[$t] = 1 }
    }
}
# Weight = 1 / doc_freq. Rare tokens ~1.0, ubiquitous ones (binary, tree) approach 0.
function Get-TokenWeight([string]$tok) {
    if ($script:docFreq.ContainsKey($tok)) { return 1.0 / $script:docFreq[$tok] }
    return 1.0  # unseen token (from folder side) counts as fully novel
}

$fuzzyCache = @{}
$fuzzyHits  = 0

function Resolve-Slug([string]$folderSlug) {
    $folderSlug = $folderSlug.ToLowerInvariant()
    if ($script:slugMap.ContainsKey($folderSlug))    { return $script:slugMap[$folderSlug] }
    if ($script:fuzzyCache.ContainsKey($folderSlug)) { return $script:fuzzyCache[$folderSlug] }

    $folderTokens = Get-SlugTokens $folderSlug
    if ($folderTokens.Count -eq 0) { $script:fuzzyCache[$folderSlug] = $null; return $null }
    $folderSet = New-Object 'System.Collections.Generic.HashSet[string]'
    foreach ($t in $folderTokens) { [void]$folderSet.Add($t) }

    $bestScore = 0.0; $bestKey = $null; $secondScore = 0.0
    foreach ($k in $script:slugTokens.Keys) {
        $catTokens = $script:slugTokens[$k]
        if ($catTokens.Count -eq 0) { continue }

        $wInter = 0.0; $wUnion = 0.0
        # Union = every catalog token (each contributes its weight)
        foreach ($tok in $catTokens) { $wUnion += Get-TokenWeight $tok }
        # Add folder-only tokens (tokens in folder but not in catalog) to union
        foreach ($tok in $folderTokens) {
            if ($catTokens -notcontains $tok) { $wUnion += Get-TokenWeight $tok }
        }
        # Intersection = tokens present in both
        foreach ($tok in $catTokens) {
            if ($folderSet.Contains($tok)) { $wInter += Get-TokenWeight $tok }
        }

        if ($wInter -le 0) { continue }
        $score = $wInter / $wUnion
        if     ($score -gt $bestScore)   { $secondScore = $bestScore; $bestScore = $score; $bestKey = $k }
        elseif ($score -gt $secondScore) { $secondScore = $score }
    }

    # Require: (a) meaningful similarity, (b) strictly better than runner-up
    if ($bestKey -and $bestScore -ge 0.4 -and $bestScore -gt $secondScore) {
        Write-Host ("    fuzzy: {0}  ->  {1}  ({2:P0})" -f $folderSlug, $bestKey, $bestScore) -ForegroundColor DarkCyan
        $script:fuzzyCache[$folderSlug] = $script:slugMap[$bestKey]
        $script:fuzzyHits++
        return $script:slugMap[$bestKey]
    }

    $script:fuzzyCache[$folderSlug] = $null
    return $null
}

# ---------------------------------------------------------------- scan
$sources = @()
foreach ($sf in $data.sourceFolders) {
    $full = if ($sf -eq '.') { $RepoRoot } else { Join-Path $RepoRoot $sf }
    if (Test-Path $full) { $sources += (Resolve-Path $full).Path }
}
if ($sources.Count -eq 0) { $sources = @($RepoRoot) }

Write-Host "-> Scanning:" -ForegroundColor Cyan
$sources | ForEach-Object { Write-Host "     $_" }

$problemFolders = @()
foreach ($src in $sources) {
    $problemFolders += Get-ChildItem -Path $src -Directory -Recurse -Depth 3 -ErrorAction SilentlyContinue |
        Where-Object { $_.Name -notmatch '^(\.git|\.github|node_modules|\.vscode)$' } |
        Where-Object {
            (Get-ChildItem $_.FullName -File -ErrorAction SilentlyContinue |
                Where-Object { $_.Name -like 'submission-*' }).Count -gt 0
        }
}
$problemFolders = $problemFolders | Sort-Object FullName -Unique

# ---------------------------------------------------------------- aggregate
$totalSolved = 0
$byDiff  = @{ Easy = 0; Medium = 0; Hard = 0 }
$byLang  = @{ cpp = 0; cs = 0; other = 0 }
$fileDates = New-Object 'System.Collections.Generic.List[datetime]'
$unmatched = New-Object 'System.Collections.Generic.List[string]'
$times     = New-Object 'System.Collections.Generic.List[int]'
$hintsFree = 0

foreach ($pf in $problemFolders) {
    $slug = $pf.Name.ToLowerInvariant()
    $files = Get-ChildItem $pf.FullName -File -ErrorAction SilentlyContinue |
             Where-Object { $_.Name -like 'submission-*' }
    if ($files.Count -eq 0) { continue }

    $totalSolved++
    $meta = Resolve-Slug $slug
    if ($meta) {
        $byDiff[$meta.Difficulty]++
        $topicSolved[$meta.Topic]++
    } else {
        [void]$unmatched.Add("$($pf.Name)   [$($pf.FullName)]")
    }

    foreach ($f in $files) {
        $ext = $f.Extension.ToLower()
        if ($ext -in @('.cpp', '.cc', '.cxx', '.hpp', '.h')) { $byLang.cpp++ }
        elseif ($ext -eq '.cs')                              { $byLang.cs++ }
        else                                                 { $byLang.other++ }

        # Date: prefer git first-commit date when -UseGit
        $date = $null
        if ($UseGit) {
            $rel = Resolve-Path -Relative $f.FullName -ErrorAction SilentlyContinue
            $gitOut = & git -C $RepoRoot log --diff-filter=A --follow --format=%aI -- "$($f.FullName)" 2>$null
            if ($LASTEXITCODE -eq 0 -and $gitOut) {
                $first = ($gitOut -split "`n" | Where-Object { $_ } | Select-Object -Last 1)
                if ($first) { $date = [datetime]::Parse($first).Date }
            }
        }
        if (-not $date) { $date = $f.LastWriteTime.Date }
        $fileDates.Add($date)

        # Parse optional header meta from first 10 lines
        try {
            $head = Get-Content $f.FullName -TotalCount 10 -ErrorAction SilentlyContinue
            foreach ($line in $head) {
                if ($line -match 'Time\s*:\s*(?<t>\d+)\s*min')       { $times.Add([int]$Matches.t) }
                if ($line -match 'Hints used\s*:\s*(?<h>yes|no)' -and $Matches.h -eq 'no') { $hintsFree++ }
            }
        } catch {}
    }
}

# ---------------------------------------------------------------- streaks
$uniqueDates = $fileDates | Sort-Object -Unique
$daysActive  = $uniqueDates.Count

$currentStreak = 0
if ($uniqueDates.Count -gt 0) {
    $today = (Get-Date).Date
    $desc  = @($uniqueDates | Sort-Object -Descending)
    # Grace: if you solved today OR yesterday, streak is alive
    $expect = $today
    if ($desc[0] -lt $today) { $expect = $desc[0] }
    foreach ($d in $desc) {
        if ($d -eq $expect) { $currentStreak++; $expect = $expect.AddDays(-1) }
        else { break }
    }
    # If latest solve was >1 day ago, streak is 0
    if (($today - $desc[0]).Days -gt 1) { $currentStreak = 0 }
}

$longestStreak = 0
$run = 0; $prev = $null
foreach ($d in ($uniqueDates | Sort-Object)) {
    if ($null -ne $prev -and ($d - $prev).TotalDays -eq 1) { $run++ } else { $run = 1 }
    if ($run -gt $longestStreak) { $longestStreak = $run }
    $prev = $d
}

# ---------------------------------------------------------------- pace
$started        = if ($uniqueDates.Count -gt 0) { ($uniqueDates | Sort-Object)[0] } else { $null }
$daysSinceStart = if ($started) { ((Get-Date).Date - $started).Days + 1 } else { 0 }
$avgPerDay      = if ($daysSinceStart -gt 0) { [math]::Round($totalSolved / $daysSinceStart, 2) } else { 0 }
$remaining      = 150 - $totalSolved
$pace           = if ($avgPerDay -gt 0) { $avgPerDay } else { 2.0 }
$etaDays        = [math]::Ceiling($remaining / $pace)
$etaDate        = (Get-Date).Date.AddDays($etaDays).ToString('yyyy-MM-dd')
$avgTime        = if ($times.Count -gt 0) { [math]::Round(($times | Measure-Object -Average).Average, 1) } else { $null }

# ---------------------------------------------------------------- totals from catalog
$diffTotals = @{ Easy = 0; Medium = 0; Hard = 0 }
foreach ($t in $data.topics) { foreach ($p in $t.problems) { $diffTotals[$p.difficulty]++ } }

# ---------------------------------------------------------------- helpers
function New-Bar([int]$done, [int]$total, [int]$width = 15) {
    if ($total -le 0) { return '' }
    $ratio  = [math]::Min(1.0, $done / $total)
    $filled = [int][math]::Round($ratio * $width)
    if ($filled -gt $width) { $filled = $width }
    return ([string]([char]0x25B0) * $filled) + ([string]([char]0x25B1) * ($width - $filled))
}
function Get-Share([int]$n, [int]$total) {
    if ($total -le 0) { return '0%' }
    return "$([int][math]::Round(100.0 * $n / $total))%"
}
function New-Anchor([string]$idx, [string]$name) {
    $a = $name.ToLower() -replace ' ', '-'
    $a = $a -replace '[^a-z0-9\-]', ''
    return "#$idx-$a"
}

# ---------------------------------------------------------------- STATS block
$startedStr = if ($started) { $started.ToString('yyyy-MM-dd') } else { 'TBD' }
$avgTimeStr = if ($null -eq $avgTime) { 'not tracked yet' } else { "$avgTime min" }
$etaStr     = if ($totalSolved -ge 150) { 'DONE!' } else { $etaDate }
$paceEmoji  = if ($totalSolved -eq 0) { '' } elseif ($avgPerDay -ge 2) { ' OK' } else { ' behind' }

$diffBarE = New-Bar $byDiff.Easy   $diffTotals.Easy   15
$diffBarM = New-Bar $byDiff.Medium $diffTotals.Medium 15
$diffBarH = New-Bar $byDiff.Hard   $diffTotals.Hard   15

$langTotal   = $byLang.cpp + $byLang.cs + $byLang.other
$progressUrl = "https://progress-bar.xyz/$totalSolved/?scale=150&title=Solved&width=500&color=00b894&suffix=%20/%20150"

$statsBlock = @"
<div align="center">

![Progress]($progressUrl)

</div>

### Overall

| Metric | Value | Target |
|:---|:---:|:---:|
| 🎯 **Solved** | ``$totalSolved / 150`` | ``150`` |
| 📅 **Days active** | ``$daysActive`` | — |
| 📈 **Avg / day** | ``$avgPerDay$paceEmoji`` | ``>= 2.0`` |
| 🔥 **Current streak** | ``$currentStreak days`` | keep alive |
| 🏆 **Longest streak** | ``$longestStreak days`` | beat it |
| ⏱️ **Avg time / problem** | ``$avgTimeStr`` | ``< 30 min`` |
| 🔁 **Solved without hints** | ``$hintsFree`` | maximize |
| 📅 **Started** | ``$startedStr`` | — |
| 🏁 **ETA at current pace** | ``$etaStr`` | — |

### By difficulty

| | 🟢 Easy | 🟡 Medium | 🔴 Hard |
|:--|:-:|:-:|:-:|
| **Solved** | ``$($byDiff.Easy) / $($diffTotals.Easy)`` | ``$($byDiff.Medium) / $($diffTotals.Medium)`` | ``$($byDiff.Hard) / $($diffTotals.Hard)`` |
| **Progress** | ``$diffBarE`` | ``$diffBarM`` | ``$diffBarH`` |

### By language

| Language | Solved | Share |
|:--|:-:|:-:|
| ![cpp](https://img.shields.io/badge/-C++-00599C?style=flat-square&logo=cplusplus&logoColor=white) | ``$($byLang.cpp)`` | ``$(Get-Share $byLang.cpp $langTotal)`` |
| ![cs](https://img.shields.io/badge/-C%23-239120?style=flat-square&logo=csharp&logoColor=white) | ``$($byLang.cs)`` | ``$(Get-Share $byLang.cs $langTotal)`` |

_Last auto-update: $((Get-Date).ToString('yyyy-MM-dd HH:mm')) · Run ``Update-Stats.ps1`` to refresh._
"@

# ---------------------------------------------------------------- ROADMAP block
$rows = New-Object 'System.Collections.Generic.List[string]'
$i = 1
foreach ($t in $data.topics) {
    $done = $topicSolved[$t.name]
    $tot  = $topicCounts[$t.name]
    $bar  = ([string]([char]0x25B0) * $done) + ([string]([char]0x25B1) * ($tot - $done))
    $anchor = New-Anchor $i $t.name
    $rows.Add("| $i | [$($t.name)]($anchor) | $tot | ``$bar`` $done/$tot |")
    $i++
}

$roadmapBlock = @"
<div align="center">

| # | Topic | Count | Progress |
|:-:|:--|:-:|:-:|
$($rows -join "`r`n")

</div>
"@

# ---------------------------------------------------------------- write README
$readme = Get-Content $ReadmePath -Raw

$statsPattern   = '(?s)(<!-- AUTO:STATS:START[^\n]*-->\r?\n).*?(\r?\n<!-- AUTO:STATS:END -->)'
$roadmapPattern = '(?s)(<!-- AUTO:ROADMAP:START[^\n]*-->\r?\n).*?(\r?\n<!-- AUTO:ROADMAP:END -->)'

if ($readme -notmatch $statsPattern) {
    Write-Warning "AUTO:STATS markers not found in README - stats block NOT updated."
} else {
    $readme = [regex]::Replace($readme, $statsPattern, {
        param($m) $m.Groups[1].Value + $script:statsBlock + $m.Groups[2].Value
    })
}

if ($readme -notmatch $roadmapPattern) {
    Write-Warning "AUTO:ROADMAP markers not found in README - roadmap block NOT updated."
} else {
    $readme = [regex]::Replace($readme, $roadmapPattern, {
        param($m) $m.Groups[1].Value + $script:roadmapBlock + $m.Groups[2].Value
    })
}

# Write UTF-8 without BOM (better for GitHub rendering)
[System.IO.File]::WriteAllText($ReadmePath, $readme, [System.Text.UTF8Encoding]::new($false))

# ---------------------------------------------------------------- report
Write-Host ""
Write-Host "== Stats ==" -ForegroundColor Green
Write-Host ("  Solved         : {0} / 150" -f $totalSolved)
Write-Host ("  Easy           : {0} / {1}" -f $byDiff.Easy,   $diffTotals.Easy)
Write-Host ("  Medium         : {0} / {1}" -f $byDiff.Medium, $diffTotals.Medium)
Write-Host ("  Hard           : {0} / {1}" -f $byDiff.Hard,   $diffTotals.Hard)
Write-Host ("  C++ files      : {0}" -f $byLang.cpp)
Write-Host ("  C#  files      : {0}" -f $byLang.cs)
Write-Host ("  Other files    : {0}" -f $byLang.other)
Write-Host ("  Days active    : {0}" -f $daysActive)
Write-Host ("  Current streak : {0}" -f $currentStreak)
Write-Host ("  Longest streak : {0}" -f $longestStreak)
Write-Host ("  Avg / day      : {0}" -f $avgPerDay)
Write-Host ("  ETA            : {0}" -f $etaStr)
Write-Host ("  Fuzzy matches  : {0}" -f $fuzzyHits)

if ($unmatched.Count -gt 0) {
    Write-Host ""
    Write-Warning "$($unmatched.Count) folder(s) had submissions but could not be matched (exact or fuzzy) to any slug in problems.json:"
    $unmatched | ForEach-Object { Write-Host "    $_" -ForegroundColor Yellow }
    Write-Host ""
    Write-Host "  Fix: open problems.json and set the 'slug' value close to your folder name (case-insensitive)." -ForegroundColor Yellow
    Write-Host "  A ~50%+ token overlap is usually enough to auto-match on future runs." -ForegroundColor Yellow
}

Write-Host ""
Write-Host "OK  README updated: $ReadmePath" -ForegroundColor Green
