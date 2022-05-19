# Update About dialog build date (build.yml)

# sed ModelViewer.rc
$date_compiled = "$((Get-Date -UFormat '%m/%d/%Y (compiled %b %d %Y %T)'))"
$file = "$PSScriptRoot\ModelViewer\ModelViewer.rc"
(Get-Content $file) | Foreach-Object {
    $_ -replace '"(?<month>[0-1][1-9])\/(?<day>[0-3][1-9])\/(?<year>\d{4}) \(.*\)"', """$date_compiled"""
} | Set-Content $file
