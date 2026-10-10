# tools/stress.ps1 -- stress-test harness (random data + brute force comparison)
#
# Usage (from project root):
#   tools\stress.cmd -Dir "????\??\???"
#   tools\stress.cmd -Dir "..." -Rounds 1000 -Sol "sol.cpp" -Brute "brute.cpp" -Gen "gen.cpp"
#
# Contract:
#   gen.cpp    generates one random test case on stdout          -- YOU write it
#   sol.cpp    your solution (auto-detected: ??.cpp / ?.cpp / Untitled1.cpp / sol.cpp)
#   brute.cpp  brute force reference for small inputs            -- YOU write it
#
# This script only compiles, loops, compares and saves the counterexample.
# It contains no solution code and never generates any.
#
# NOTE: keep this file ASCII-only. Windows PowerShell 5.1 reads non-BOM
#       non-ASCII .ps1 files as ANSI and mangles Chinese text.

param(
    [Parameter(Mandatory = $true)][string]$Dir,
    [int]$Rounds = 500,
    [string]$Gen = "gen.cpp",
    [string]$Sol = "",
    [string]$Brute = "brute.cpp",
    [int]$TimeLimitSec = 5,
    [string]$Gxx = "",
    [switch]$KeepArtifacts
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
$work = if ([System.IO.Path]::IsPathRooted($Dir)) { $Dir } else { Join-Path $root $Dir }

function Say($msg) { Write-Host $msg }
function Die($msg) { Write-Host ""; Write-Host "ERROR: $msg" -ForegroundColor Red; exit 1 }

if (-not (Test-Path $work)) { Die "Directory not found: $work" }
$work = (Resolve-Path $work).Path
Say "Work dir: $work"

# ---------- locate compiler ----------
if (-not $Gxx) {
    if ($env:STRESS_GXX) { $Gxx = $env:STRESS_GXX }
    elseif (Test-Path "C:\CSP_SIM\tools\mingw64\bin\g++.exe") { $Gxx = "C:\CSP_SIM\tools\mingw64\bin\g++.exe" }
    else {
        $c = Get-Command g++ -ErrorAction SilentlyContinue
        if ($c) { $Gxx = $c.Source }
    }
}
if (-not $Gxx -or -not (Test-Path $Gxx)) { Die "g++ not found. Pass -Gxx <path> or set STRESS_GXX." }
Say "Compiler: $Gxx"

# ---------- locate solution ----------
# Preferred names, written as .NET regex \u escapes so this file stays ASCII
# (PowerShell 5.1 mangles non-ASCII in non-BOM .ps1 files):
#   \u4EE3\u7801 = ??.cpp ,  \u6E90 = ?.cpp
if (-not $Sol) {
    $rx = [regex]'^(\u4EE3\u7801|\u6E90|sol|Untitled\d*|main)\.cpp$'
    $cands = @(Get-ChildItem -LiteralPath $work -Filter *.cpp -File |
        Where-Object { $rx.IsMatch($_.Name) -and $_.Name -ne $Gen -and $_.Name -ne $Brute } |
        Sort-Object Name)
    if ($cands.Count -ge 1) { $Sol = $cands[0].Name }
}
if (-not $Sol) { Die "Solution source not found. Pass -Sol <file> (e.g. -Sol sol.cpp)." }

$genPath = Join-Path $work $Gen
$solPath = Join-Path $work $Sol
$brutePath = Join-Path $work $Brute

if (-not (Test-Path -LiteralPath $genPath)) {
    Say ""
    Say "Missing generator: $Gen"
    Say "Write gen.cpp yourself in: $work"
    Say "  - keep data SMALL so the brute force finishes (e.g. n <= 10, values in [-20, 20])"
    Say "  - randomize with rand()/mt19937, print one complete test case"
    Say "  - cover edges: n=1, all equal, all negative, no-solution cases"
    exit 1
}
if (-not (Test-Path -LiteralPath $brutePath)) {
    Say ""
    Say "Missing brute force: $Brute"
    Say "Write brute.cpp yourself in: $work (enumerate/search for the exact answer on small data)."
    Say "Hint: brute force may be as slow as you like, as long as small cases finish."
    exit 1
}

# ---------- compile ----------
$tmp = Join-Path $env:TEMP ("stress_" + [System.Guid]::NewGuid().ToString("N").Substring(0, 8))
New-Item -ItemType Directory -Path $tmp | Out-Null
$exeGen = Join-Path $tmp "gen.exe"
$exeSol = Join-Path $tmp "sol.exe"
$exeBrute = Join-Path $tmp "brute.exe"

function Build($src, $out, $label) {
    Say "Compiling $label ..."
    $outText = & $Gxx -std=c++17 -O2 -o $out $src 2>&1
    if ($LASTEXITCODE -ne 0) {
        Write-Host ($outText -join "`n") -ForegroundColor Red
        Die "$label failed to compile"
    }
}
Build $genPath $exeGen "gen"
Build $solPath $exeSol "sol"
Build $brutePath $exeBrute "brute"

# ---------- run helper (with timeout) ----------
function RunCase($exe, $inFile, $timeoutSec) {
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $exe
    $psi.RedirectStandardInput = $true
    $psi.RedirectStandardOutput = $true
    $psi.UseShellExecute = $false
    $psi.CreateNoWindow = $true
    $p = [System.Diagnostics.Process]::Start($psi)
    if ($inFile) {
        $sr = New-Object System.IO.StreamReader($inFile)
        $p.StandardInput.Write($sr.ReadToEnd())
        $sr.Close()
    }
    $p.StandardInput.Close()
    $out = $p.StandardOutput.ReadToEnd()
    if (-not $p.WaitForExit($timeoutSec * 1000)) {
        try { $p.Kill() } catch { }
        return @{ Timeout = $true; ExitCode = -1; Output = $out }
    }
    return @{ Timeout = $false; ExitCode = $p.ExitCode; Output = $out }
}

function Norm($text) {
    $t = $text -replace "`r`n", "`n" -replace "`r", "`n"
    $lines = $t -split "`n" | ForEach-Object { $_.TrimEnd() }
    $list = New-Object System.Collections.Generic.List[string]
    foreach ($l in $lines) { $list.Add($l) }
    while ($list.Count -gt 0 -and $list[$list.Count - 1] -eq "") { $list.RemoveAt($list.Count - 1) }
    return ($list -join "`n")
}

$inFile = Join-Path $tmp "in.txt"

Say ""
Say "Stress testing: $Rounds rounds, ${TimeLimitSec}s per run ..."
$bad = $null
for ($i = 1; $i -le $Rounds; $i++) {
    $g = RunCase $exeGen $null $TimeLimitSec
    if ($g.Timeout) { $bad = @{ Round = $i; Reason = "generator timed out (infinite loop?)" }; break }
    [System.IO.File]::WriteAllText($inFile, $g.Output, [System.Text.Encoding]::UTF8)

    $r1 = RunCase $exeSol $inFile $TimeLimitSec
    if ($r1.Timeout) { $bad = @{ Round = $i; Reason = "solution timed out (> ${TimeLimitSec}s)" }; break }
    if ($r1.ExitCode -ne 0) { $bad = @{ Round = $i; Reason = "solution crashed (exit code $($r1.ExitCode)) - out of bounds / division by zero / stack overflow?" }; break }

    $r2 = RunCase $exeBrute $inFile ($TimeLimitSec * 4)
    if ($r2.Timeout) { $bad = @{ Round = $i; Reason = "brute force timed out - make gen.cpp produce smaller data" }; break }

    $a = Norm $r1.Output
    $b = Norm $r2.Output
    if ($a -ne $b) { $bad = @{ Round = $i; Reason = "answers differ"; Sol = $a; Brute = $b }; break }

    if ($i % 50 -eq 0) { Say ("  passed {0}/{1}" -f $i, $Rounds) }
}

Say ""
if ($bad) {
    Write-Host ("FAILED at round {0}: {1}" -f $bad.Round, $bad.Reason) -ForegroundColor Red
    $failIn = Join-Path $work "_fail.in"
    Copy-Item $inFile $failIn -Force
    Say "Counterexample saved to: $failIn"
    Say ""
    Say "--- input ---"
    Get-Content $failIn
    if ($bad.ContainsKey("Sol")) {
        Say "--- your solution ---"; Say $bad.Sol
        Say "--- brute force ---";    Say $bad.Brute
    }
    Say ""
    Say "Next: shrink this input to a minimal case, run both programs by hand, find the divergence."
    if (-not $KeepArtifacts) { Remove-Item $tmp -Recurse -Force -ErrorAction SilentlyContinue }
    exit 1
}

Write-Host ("ALL PASSED: {0} random cases, solution matches brute force." -f $Rounds) -ForegroundColor Green
Say ""
Say "Reminder: passing random tests does not prove correctness. Also test extreme cases"
Say "(n=1, all equal, upper bounds) by hand."
if (-not $KeepArtifacts) { Remove-Item $tmp -Recurse -Force -ErrorAction SilentlyContinue }
exit 0
