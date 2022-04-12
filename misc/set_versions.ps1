# create headers dictionary
# $h = @{"Authorization" = "token ${{secrets.GITHUB_TOKEN}}"}

# try {
#     $response = Invoke-WebRequest -Uri ${{env.RELEASES_URL}}/latest -Headers $h -Method GET
#     $json = $response.Content | ConvertFrom-Json
#     $tag_name = $json.tag_name
# }
# catch {
#     # none so far
#     $tag_name = "0.0"
# }

# $next_version = "1.0.0"
# if ($tag_name -match '(?<major>0|[1-9]\d*)\.(?<minor>0|[1-9]\d*)(?:\.(?<patch>0|[1-9]\d*))?$') {
#     if ($Matches.ContainsKey('patch')) {
#       $patch = 1 + $Matches['patch']
#     }
#     else {
#       $patch = 0
#     }
#     $next_version = $Matches['major'] + "." + $Matches['minor'] + "." + $patch
# }

# major.minor.patch.build

##$build = ${{github.run_number}}
$next_version = "1.0.0"
$build = "77"

$fullversion = "$next_version.$build"
$fullversion_commas = $fullversion.Replace(".", ",")
$date_compiled = "$((Get-Date -UFormat '%m/%d/%Y (compiled %b %d %Y %T)'))"

# sed CMakeLists.txt
$file = ".\CMakeLists.txt"
(Get-Content $file) | Foreach-Object {
    $_ -replace 'VERSION (?<major>0|[1-9]\d*)\.(?<minor>0|[1-9]\d*)(?:\.(?<patch>0|[1-9]\d*))', "VERSION $next_version"
} | Set-Content $file

# sed ModelViewer.rc
$file = ".\ModelViewer\ModelViewer.rc"
(Get-Content $file) | Foreach-Object {
  $_ -replace 'Version (?<major>0|[1-9]\d*)\.(?<minor>0|[1-9]\d*)(?:\.(?<patch>0|[1-9]\d*))',                                    "Version $next_version"`
      -replace 'FILEVERSION (?<major>0|[1-9]\d*),(?<minor>0|[1-9]\d*),(?<patch>0|[1-9]\d*),(?<build>0|[1-9]\d*)',                 "FILEVERSION $fullversion_commas"`
      -replace 'PRODUCTVERSION (?<major>0|[1-9]\d*),(?<minor>0|[1-9]\d*),(?<patch>0|[1-9]\d*),(?<build>0|[1-9]\d*)',              "PRODUCTVERSION $fullversion_commas"`
      -replace 'VALUE "FileVersion", "(?<major>0|[1-9]\d*)\.(?<minor>0|[1-9]\d)\.(?<patch>0|[1-9]\d*)\.(?<build>0|[1-9]\d*)"',    "VALUE ""FileVersion"", ""$fullversion"""`
      -replace 'VALUE "ProductVersion", "(?<major>0|[1-9]\d*)\.(?<minor>0|[1-9]\d)\.(?<patch>0|[1-9]\d*)\.(?<build>0|[1-9]\d*)"', "VALUE ""ProductVersion"", ""$fullversion"""`
      -replace '"(?<month>[0-1][1-9])\/(?<day>[0-3][1-9])\/(?<year>\d{4}) \(.*\)"',                                               """$date_compiled"""`
} | Set-Content $file

# set env for remaining steps
if ($env:GITHUB_REPOSITORY -eq 'MODFLOW-USGS/modelviewer-mf6') {
  Write-Output "DIST_DIR=mvmf6-$next_version" >> $env:GITHUB_ENV
} else {
  Write-Output "DIST_DIR=mvmf6-$env:GITHUB_REPOSITORY_OWNER-$next_version" >> $env:GITHUB_ENV
}
Write-Output "NEXT_VERSION=$next_version" >> $env:GITHUB_ENV
