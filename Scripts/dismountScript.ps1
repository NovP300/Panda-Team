# Проверка аргумента
if ($args.Count -lt 1) {
    Write-Host "Error: Please specify the path to the JSON file."
    Write-Host "Example: .\dismountScript.ps1 path\to\disk.json"
    exit 1
}

# Получение пути к JSON-файлу
$jsonFilePath = $args[0]

# Проверка существования JSON-файла
if (-Not (Test-Path $jsonFilePath)) {
    Write-Host "Error: The specified file '$jsonFilePath' does not exist."
    exit 1
}

# Чтение JSON-файла
$jsonObject = Get-Content $jsonFilePath | ConvertFrom-Json

# Выполнение команды aim_cli для размонтирования всех виртуальных дисков
$output = aim_cli /dismount 

# Проверка, если команда выполнена успешно
if ($output -match "All devices dismounted") {
    Write-Host "All virtual disks have been successfully dismounted."

    # Обнуляем буквы всех разделов в JSON
    foreach ($partition in $jsonObject.Partitions) {
        $partition.Letter = ""
    }

    # Сохраняем обновленный JSON обратно в файл
    $jsonObject | ConvertTo-Json -Depth 10 | Set-Content $jsonFilePath -Encoding UTF8
    Write-Host "The partition letters have been successfully cleared in $jsonFilePath"
} 
else {
    Write-Host "Error during dismounting disks. Output: $output"
}

