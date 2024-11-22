# Проверяем, был ли передан аргумент
if ($args.Count -eq 0) {
    Write-Host "Error: Please specify the path to the JSON file when running the script!"
    Write-Host "Example: .\mountScript.ps1 path\to\disk.json"
    exit 1
}

# Получаем путь к JSON-файлу из аргумента
$jsonFilePath = $args[0]

# Проверяем, существует ли JSON-файл
if (-Not (Test-Path $jsonFilePath)) {
    Write-Host "Error: The file '$jsonFilePath' does not exist."
    exit 1
}

# Читаем JSON-файл
$jsonObject = Get-Content $jsonFilePath | ConvertFrom-Json

# Получаем путь к виртуальному диску из JSON-объекта
$vdiskPath = $jsonObject.path

# Проверяем, существует ли виртуальный диск
if (-Not (Test-Path $vdiskPath)) {
    Write-Host "Error: The disk image '$vdiskPath' does not exist."
    exit 1
}

# Инициализация индекса для перебора разделов в JSON
$i = 0

try {
    # Монтируем образ с помощью aim_cli
    aim_cli /mount:removable /online /writable /filename="$vdiskPath" /background

    # Даем системе время для монтирования
    Start-Sleep -Seconds 2

    # Получаем информацию о смонтированных томах из aim_cli --list
    $output = aim_cli --list

    # Ищем строки с буквами дисков (например, "Mounted at X:/")
    $mountedLines = $output | Select-String -Pattern "Mounted at"

    # Обрабатываем каждую строку и записываем буквы в JSON
    foreach ($line in $mountedLines) {
        # Извлекаем букву диска из строки
        $diskLetter = $line -replace "Mounted at (\w):\\.*", '$1'

        # Проверяем, что индекс находится в пределах диапазона разделов в JSON
        if ($i -lt $jsonObject.Partitions.Count) {
            # Присваиваем букву диска соответствующему разделу
            $jsonObject.Partitions[$i].Letter = $diskLetter.Trim()
        } else {
            Write-Host "Warning: More partitions are mounted than defined in the JSON file."
        }

        # Увеличиваем индекс для следующего раздела
        $i++
    }
} catch {
    Write-Host "Error while mounting '$vdiskPath'. Error: $_"
    exit 1
}

# Сохраняем обновленный JSON обратно в файл
$jsonObject | ConvertTo-Json -Depth 10 | Set-Content $jsonFilePath -Encoding UTF8

# Выводим сообщение об успехе
Write-Host "Drive letters have been successfully recorded in $jsonFilePath"
