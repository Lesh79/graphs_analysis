# Анализ графовых алгоритмов

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## Описание

Учебный проект по анализу производительности графовых алгоритмов с использованием библиотек **SuiteSparse::GraphBLAS**, **LAGraph** и **SPLA**.

## Структура проекта
```bash
.
├── cpp/                     # Исходный код проекта
│   ├── graphblas/           # Алгоритмы на SuiteSparse::GraphBLAS и LAGraph
│   └── spla/                # Алгоритмы на SPLA
│
├── configs/                 # Конфигурации алгоритмов
│
│
├── experiments/             # Скрипты для проведения экспериментов
│
│
├── results/                 # Результаты экспериментов (.txt)
│
├── graphics/                # Визуализация и анализ результатов экспериментов
│
│
├── flamegraphs/             # Результаты профилирования
│   ├── *.svg                # Flame Graphs
│
│
├── slides/                  # Презентации
```

## Рассматриваемые алгоритмы

1. **Single-source parent BFS**
    - **GraphBLAS** (Нурмухаметов Рафик)
    - **SPLA** (Сыресенков Илья)
2. **Multiple-source parent BFS**
    - **GraphBLAS** (Нурмухаметов Рафик)
    - **SPLA** (Сыресенков Илья)
3. **PageRank**
    - **GraphBLAS** (Нурмухаметов Рафик)
    - **SPLA** (Сыресенков Илья)
4. **MST: Borůvka**
    - **GraphBLAS** (Нурмухаметов Рафик)
    - **SPLA** (Сыресенков Илья)

## Эксперименты

Каждый алгоритм в каждом эксперименте запускался 15 раз.

### Эксперимент 1

**Цель:** для каждого представленного алгоритма сравнить среднее время выполнения для реализаций с использованием **GraphBLAS** и **SPLA**:

**Примечание:** для **MS Parent BFS** количество стартовых вершин будет фиксированным: $256$.

### Эксперимент 2

**Цель:** оценить замедление **MS Parent BFS** при увеличении числа стартовых вершин. Количество стартовых вершин: $1, 4, 16, 64, 256$. Выбираться стартовые вершины будут случайно с фиксированным сидом.

### Датасет
Для эксперимента с алгоритмом **PageRank** графы были взяты из [Stanford Large Network Dataset Collection](https://snap.stanford.edu/data/#socnets).

Для всех остальных экспериментов графы были взяты из [9th DIMACS Implementation Challenge – Shortest Paths](https://www.diag.uniroma1.it/challenge9/).

Подробнее с набором данных для проведения экспериметов можно ознакомиться по [ссылке](https://drive.google.com/drive/folders/1_l20PoxHFo0VZN1QVwZ5eRTyqenlpwlC).

### Результаты
Ознакомиться с результатами эксперимента можно в [презентации](slides/bfs_pagerank_boruvka_second.pdf).

Посмотреть «сырые» результаты можно в директориях [`results`](results/) и [`graphics`](graphics/).
