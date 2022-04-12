$prelim_pt1 = '"This software is preliminary or provisional and is subject to revision. It is being provided to meet the need for timely best science. The software has not received final approval by the U.S. Geological Survey \(USGS\).",.*$'
$prelim_pt2 = '"No warranty, expressed or implied, is made by the USGS or the U.S. Government as to the functionality of the software and related material nor shall the fact of release constitute any such warranty.",.*$'
$prelim_pt3 = '"The software is provided on the condition that neither the USGS nor the U.S. Government shall be held liable for any damages resulting from the authorized or unauthorized use of the software.",.*$'
$approved_pt1 = '"This software has been approved for release by the U.S. Geological Survey (USGS). Although the software has been subjected to rigorous review, the USGS reserves the right to update the software as needed pursuant to further analysis and review.",IDC_STATIC,15,110,189,40'
$approved_pt2 = '"No warranty, expressed or implied, is made by the USGS or the U.S. Government as to the functionality of the software and related material nor shall the fact of release constitute any such warranty.",IDC_STATIC,15,152,190,34'
$approved_pt3 = '"Furthermore, the software is released on condition that neither the USGS nor the U.S. Government shall be held liable for any damages resulting from its authorized or unauthorized use.",IDC_STATIC,15,187,190,26'
# sed ModelViewer.rc
$file = ".\ModelViewer\ModelViewer.rc"
(Get-Content $file) | Foreach-Object {
    $_ -replace $prelim_pt1, $approved_pt1`
        -replace $prelim_pt2, $approved_pt2`
        -replace $prelim_pt3, $approved_pt3`
} | Set-Content $file