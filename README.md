# 2d-approximation
Репозитории с визуализацией
https://github.com/wisp167/2d-approximation-visualization.git
## Описание
Многопоточно строит на области [a,b]x[c,d] линейную аппроксимацию функции по норме L1 по данной функции на равномерной сетке (количество точек аппроксимации - nx по оси X, ny по оси Y) с точностью eps, совершив не более m операций. Погрешность вычисляется в точках аппроксимации и центрах тяжести треугольников на сетке.
![net](https://github.com/user-attachments/assets/e00e5e6b-5b66-4ac9-adbb-b34f5bfba8c1)

## Описание работы
На сетке строится базис в точках аппроксимации - пирамидки (см. рисунок. Каждая захватывает не более 6 треугольников, значение равно 1 в точке аппроксимации, линейно убывает к краям треугольника. Набор таких функций является базисом для линейной аппроксимации). С помощью интегрирования находим матрицу грама (в строке i количество ненулевых элементов равно количеству элементов базиса, с которыми данный соседствует). Так же раскладываем значения данной функции по базису (но в качестве значения функции берём её среднее весовое с центрами треугольников по вкладу в функции базиса, чтобы минимизировать погрешность в точках апроксимации и центрах треугольников). Матрица грама хранится в виде MSR (modified sparse row), номера хранятся как (i, j) -> i*nx+j, разделена между потоками как по строкам в матрице грама.

![triangle](https://github.com/user-attachments/assets/12c098c5-983c-467b-b0e5-24e9e5056c45)

Задача свелась к нахождению решения системы линейных уравнений с разряженной матрицей, делаться это будет аппроксимационно, так как по-обычному слишком долго (куб от количества точек аппроскимации). Формула для итераций (можно показать, что сходится. M - предобуславливатель для лучшей сходимости (можно, например, взять M = diag(A)) t_{0}=1, x инициализируется случайно (можно нулями), r - как невязка для x):
![equation](https://latex.codecogs.com/png.image?%5Cdpi%7B110%7D%5Cbg%7Bwhite%7D%20M%5Cfrac%7Bx_%7Bn&plus;1%7D-x_%7Bn%7D%7D%7Bt_%7Bk%7D%7D&plus;Ax_%7Bn%7D=b)

![equation](https://latex.codecogs.com/png.image?%5Cdpi%7B110%7D%5Cbg%7Bwhite%7D%20r_%7Bk&plus;1%7D=r_%7Bk%7D-t_%7Bk%7DM%5E%7B-1%7Dr_%7Bk%7D)

![equation](https://latex.codecogs.com/png.image?%5Cdpi%7B110%7D%5Cbg%7Bwhite%7Dx_%7Bk&plus;1%7D=x_%7Bk%7D-t_%7Bk%7DM%5E%7B-1%7Dr_%7Bk%7D)

![equation](https://latex.codecogs.com/png.image?%5Cdpi%7B110%7D%5Cbg%7Bwhite%7Dt_%7Bk%7D=%5Cfrac%7B(M%5E%7B-1%7Dr_%7Bk%7D,r_%7Bk%7D)%7D%7B(A%20M%5E%7B-1%7Dr_%7Bk%7D,M%5E%7B-1%7Dr_%7Bk%7D)%7D)

(Если исчезли картинки: r_{k+1} = r_{k}-t_{k}M^{-1}r_{k},
x_{k+1} = x_{k}-t_{k}M^{-1}r_{k},
t_{k} = \frac{(M^{-1}r_{k}, r_{k})}{(A \mult M^{-1}r_{k}, M^{-1}r_{k})})

Используются умножение msr-матрицы на вектор, скалярное произведение векторов и домножение msr-матрицы на диагональную, каждая из операций хорошо параллелится. Во всех объединениях ответов потоков (например, в скалярном произведении) учитывается порядок их заходов в точку синхронизации, так как размеры векторов могут довольно большими (1e7 и выше), что может повлиять на ответ при высокой точности.

## Флаги
-O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
## Параметры запуска
> ./a.out a b c d nx ny k eps m p

1) [a,b]x[c,d] - область, на которой ищется аппроскимация (тип double)
2) nx, ny - количество точек аппроксимации по осям X, Y (тип int)
3) k - тип функции (тип int)
4) eps - точность (тип double)
5) m - максимальное кол-во итераций (тип int)
6) p - количество потоков (тип int)
