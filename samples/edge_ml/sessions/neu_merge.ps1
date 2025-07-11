# Define parameters
param(
    [Parameter(Mandatory=$false, Position=0)]
    [string]$FolderPath = (Split-Path -Parent $MyInvocation.MyCommand.Path)
)

# Output file name
$outputFile = Join-Path $FolderPath "merged_output.csv"

# Get all CSV files
$csvFiles = Get-ChildItem -Path $FolderPath -Filter "*.csv"

if ($csvFiles.Count -eq 0) {
    Write-Host "No CSV files found in the directory."
    exit
}

Write-Host "Found $($csvFiles.Count) CSV files to merge."

# Get first file's header
$headerLine = Get-Content -Path $csvFiles[0].FullName -TotalCount 1
Set-Content -Path $outputFile -Value $headerLine

# Append all files (skipping header for all except first)
foreach ($file in $csvFiles) {
    Write-Host "Processing: $($file.Name)"
    (Get-Content -Path $file.FullName | Select-Object -Skip 1) | Add-Content -Path $outputFile
}

Write-Host "Merged all files into $outputFile"