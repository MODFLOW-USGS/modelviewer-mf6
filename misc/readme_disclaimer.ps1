$approved = ("Disclaimer`n" +
             "----------`n" +
             "`n" +
             "This software has been approved for release by the U.S. Geological Survey`n" +
             "(USGS). Although the software has been subjected to rigorous review, the USGS`n" +
             "reserves the right to update the software as needed pursuant to further analysis`n" +
             "and review. No warranty, expressed or implied, is made by the USGS or the U.S.`n" +
             "Government as to the functionality of the software and related material nor`n" +
             "shall the fact of release constitute any such warranty. Furthermore, the`n" +
             "software is released on condition that neither the USGS nor the U.S. Government`n" +
             "shall be held liable for any damages resulting from its authorized or`n" +
             "unauthorized use.")
$file = ".\README.md"
(Get-Content -Raw $file) -replace "(?ms)Disclaimer.*", $approved | Set-Content $file
