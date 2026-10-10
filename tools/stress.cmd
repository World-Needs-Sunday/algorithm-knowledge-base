@echo off
rem Stress-test runner (bypasses the PowerShell execution policy).
rem Usage: tools\stress.cmd -Dir "????\??\???" [-Rounds 500] [-Sol ??.cpp]
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0stress.ps1" %*
