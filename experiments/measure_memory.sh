#!/bin/bash

# Список команд для выполнения
commands=(
    "./build/cpp/graphblas/ssbfs/gb-ssbfs configs/ssbfs.ini"
    "./build/cpp/spla/ssbfs/spla-ssbfs configs/ssbfs.ini"
    "./build/cpp/graphblas/msbfs/gb-msbfs configs/msbfs.ini"
    "./build/cpp/spla/msbfs/spla-msbfs configs/msbfs.ini"
    "./build/cpp/graphblas/mst/gb-mst configs/mst.ini"
    "./build/cpp/spla/mst/spla-mst configs/mst.ini"
)

output_file="memory_usage.txt"

# Очищаем файл результатов
> "$output_file"

# Функция для получения имени процесса из команды
get_process_name() {
    local cmd="$1"
    # Берем первое слово из команды (название программы)
    echo "$cmd" | awk '{print $1}'
}

# Функция для получения всей иерархии процессов
get_process_tree() {
    local pid=$1
    echo "$pid"
    # Получаем все дочерние процессы
    for child in $(ps -o pid --ppid "$pid" --no-headers 2>/dev/null); do
        get_process_tree "$child"
    done
}

# Функция для мониторинга памяти всей группы процессов
monitor_memory() {
    local pid=$1
    local duration=$2
    local max_memory=0
    local total_samples=0
    local total_memory=0
    
    for ((i=0; i<duration; i++)); do
        if kill -0 "$pid" 2>/dev/null; then
            # Получаем все процессы в дереве
            pids=$(get_process_tree "$pid")
            
            # Суммируем память всех процессов
            current_memory=0
            for process in $pids; do
                if kill -0 "$process" 2>/dev/null 2>/dev/null; then
                    rss_kb=$(ps -o rss= -p "$process" 2>/dev/null | awk '{print $1}')
                    if [[ -n "$rss_kb" && "$rss_kb" -gt 0 ]]; then
                        current_memory=$((current_memory + rss_kb))
                    fi
                fi
            done
            
            if [[ $current_memory -gt 0 ]]; then
                total_memory=$((total_memory + current_memory))
                total_samples=$((total_samples + 1))
                
                # Обновляем максимальное значение
                if [[ $current_memory -gt $max_memory ]]; then
                    max_memory=$current_memory
                fi
            fi
        else
            break
        fi
        sleep 0.5
    done
    
    # Возвращаем среднее и максимальное значение в МБ
    if [[ $total_samples -gt 0 ]]; then
        average_kb=$((total_memory / total_samples))
        average_mb=$((average_kb / 1024))
        max_mb=$((max_memory / 1024))
        echo "$max_mb:$average_mb"
    else
        echo "0:0"
    fi
}

# Выполняем команды последовательно
for cmd in "${commands[@]}"; do
    # Получаем имя процесса
    process_name=$(get_process_name "$cmd")
    
    # Запускаем команду в фоне
    eval "$cmd" &
    pid=$!
    
    # Даем процессу время на запуск
    # sleep 0.1
    
    # Мониторим потребление памяти в течение 20 секунд
    result=$(monitor_memory $pid 20)
    max_mb=$(echo "$result" | cut -d: -f1)
    average_mb=$(echo "$result" | cut -d: -f2)
    
    # Завершаем процесс
    if kill -0 "$pid" 2>/dev/null; then
        # Завершаем всю группу процессов
        kill -- -$(ps -o pgid= -p "$pid" | tr -d ' ') 2>/dev/null
        wait $pid 2>/dev/null
    fi
    
    # Записываем результат в файл в требуемом формате
    echo "${process_name}:${max_mb}:${average_mb}" >> "$output_file"
    
    # Выводим прогресс в консоль (не в файл)
    echo "Завершено: $cmd (Макс: ${max_mb} МБ, Средн: ${average_mb} МБ)"
done

echo "Готово! Результаты записаны в файл: $output_file"