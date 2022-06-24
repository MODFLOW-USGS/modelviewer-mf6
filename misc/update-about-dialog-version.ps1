# Update About dialog version (build.yml)

$env:SEM_VER_REGEX =  "^(?<major>0|[1-9]\d*)\.(?<minor>0|[1-9]\d*)\.(?<patch>0|[1-9]\d*)(?:-(?<prerelease>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+(?<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$"
$env:SEM_VER_REGEX_2 = "(?<major>0|[1-9]\d*)\.(?<minor>0|[1-9]\d*)\.(?<patch>0|[1-9]\d*)(?:-(?<prerelease>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+(?<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?"

${github.run_number} = "127"
${github.repository_owner} = $env:USERNAME

${file.json} = "$PSScriptRoot\..\code.json"
$ver = ((Get-Content ${file.json}) | ConvertFrom-Json).version
##echo "::debug::version = $ver"
##echo "::debug::SEM_VER_REGEX = $env:SEM_VER_REGEX"
if ($ver -notmatch $env:SEM_VER_REGEX) {
  echo "::error::Bad version in code.json -- $ver (see https://semver.org/)"
  exit 1
}
if ($Matches["prerelease"] -and $Matches["prerelease"] -like "*runnumber*") {
    $version = $Matches.0 -replace "runnumber", "${github.run_number}"
    $version = $version -replace "prerelease.", "prerelease-vtk-9.1-"
}
else {
    $version = $Matches.0
}
$name = "${github.repository_owner}"
$dist_dir = "mvmf6-${version}"
$user_dist_dir = "${name}-mvmf6-${version}"
# export
$env:SEMVER=$version
$env:DIST_DIR=$dist_dir
$env:USER_DIST_DIR=$user_dist_dir


# sed ModelViewer.rc
${file.rc} = "$PSScriptRoot\..\ModelViewer\ModelViewer.rc"
(Get-Content ${file.rc}) | Foreach-Object {
    $_ -replace "`"Version $env:SEM_VER_REGEX_2`"", "`"Version $env:SEMVER`""
} | Set-Content ${file.rc}
