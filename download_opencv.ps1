param(
    [string]$DownloadUrl,
    [string]$OutputPath
)

[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

Write-Host "Downloading from: $DownloadUrl"
Write-Host "Output path: $OutputPath"
Write-Host ""

try {
    $ProgressPreference = 'SilentlyContinue'
    Invoke-WebRequest -Uri $DownloadUrl -OutFile $OutputPath -UseBasicParsing
    Write-Host "Download completed successfully!"
    exit 0
}
catch {
    Write-Host "Download failed: $($_.Exception.Message)"
    exit 1
}
