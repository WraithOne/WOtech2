param(
    [string]$Scene = "Assets\Scenes\FeatureTest.json",
    [string]$Output = "dist"
)
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
$msbuild = & $vswhere -latest -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
& $msbuild "$root\WOtech2.sln" /p:Configuration=Release /p:Platform=x64 /m /v:minimal
New-Item -ItemType Directory -Force -Path "$root\$Output" | Out-Null
Copy-Item "$root\bin\x64\Release\WOtech2Runtime.exe" "$root\$Output\WOtech2Runtime.exe" -Force
Copy-Item "$root\$Scene" "$root\$Output\scene.json" -Force
'{"runtime":true,"editor":false,"scene":"scene.json"}' | Set-Content "$root\$Output\export.json"
Write-Output "Exported to $root\$Output"
