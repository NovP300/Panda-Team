# Проверка аргумента - путь к JSON файлу
if ($args.Count -eq 0) {
    Write-Host "Error: Please specify the path to the JSON file."
    Write-Host "Example: .\formatDisks.ps1 path\to\disk.json"
    exit 1
}

$jsonFilePath = $args[0]

# Проверка существования JSON-файла
if (-Not (Test-Path $jsonFilePath)) {
    Write-Host "Error: The specified file '$jsonFilePath' does not exist."
    exit 1
}

# Чтение JSON файла
$jsonObject = Get-Content $jsonFilePath | ConvertFrom-Json

# Извлечение пути к образу диска
$vdiskPath = $jsonObject.path

# Чтение информации о разделах из JSON
$partitions = $jsonObject.Partitions

foreach ($partition in $partitions) {
    # Получение буквы диска, файловой системы и метки тома
    $driveLetter = $partition.Letter
    $fileSystem = $partition.Filesystem
    $volumeLabel = $partition.DriveName

    Write-Host "Processing partition: $driveLetter, File system: $fileSystem, Volume label: $volumeLabel"

    # Формирование команд для diskpart
    $scriptBlock = @"
select volume $driveLetter
format fs=$fileSystem label="$volumeLabel" quick
"@

    # Путь к временному файлу для скрипта diskpart
    $tempScriptPath = Join-Path $PSScriptRoot "diskpart_script_$driveLetter.txt"

    # Сохранение скрипта во временный файл
    $scriptBlock | Set-Content $tempScriptPath

    # Выполнение скрипта diskpart
    diskpart /s $tempScriptPath

    # Удаление временного файла скрипта
    Remove-Item $tempScriptPath
}

Write-Host "Formatting completed!"
