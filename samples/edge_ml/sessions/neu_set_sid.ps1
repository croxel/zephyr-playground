param(
    [Parameter(Mandatory=$true, Position=0)]
    [string]$InputFile,

    [Parameter(Mandatory=$true, Position=1)]
    [string]$TextToAppend
)

# Check if the input file exists
if (-not (Test-Path $InputFile)) {
    Write-Error "Input file '$InputFile' does not exist."
    exit 1
}

try {
    # Read all lines from the file, append the text, and write back to the file
    $content = Get-Content $InputFile
    $newContent = $content | ForEach-Object { "$_$TextToAppend" }

    # Write the modified content back to the original file
    $newContent | Set-Content $InputFile -Encoding UTF8

    Write-Host "Successfully appended text to all lines in '$InputFile'"
}
catch {
    Write-Error "An error occurred: $_"
    exit 1
}