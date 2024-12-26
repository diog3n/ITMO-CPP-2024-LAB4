# Sparse matrix in C++
Разреженная матрица, позволяющая хранить только не равные нулю элементы.

# Возможные операции
На данный момент реализованы следующие операции:
* Сложение
* Вычитание
* Умножение (в т.ч. с вектором)
* Возведение в степень
* Вычисление определителя
* Обращение
* Транспонирование
* Поэлементное сложение со скаляром
* Поэлементное вычитание скаляра
* Поэлементное умножение на скаляр
* Поэлементное возведение в степень
# Сборка
Просто запустите
```
make
```
# Вывод
При запуске программа выдаст результаты прохождения тестов (по операциям):
```
Test smatrix equality OK!
Test smatrix sum OK!
Test smatrix diff OK!
Test smatrix multiplication OK!
Test smatrix-vector multiplication OK!
Test smatrix determinant OK!
Test smatrix inverse OK!
Test smatrix transpose OK!
Test smatrix scalar operations OK!
Test smatrix power OK!
Test vmatrix equality OK!
Test vmatrix sum OK!
Test vmatrix diff OK!
Test vmatrix multiplication OK!
vmat1 real size is 10000
smat1 real size is 6600
vmat2 real size is 10000
smat2 real size is 6666
VectorMatrix test start
VectorMatrix multiplication took 4528154 nanoseconds.
SparseMatrix test start
SparseMatrix multiplication took 4035281760 nanoseconds.
vresult real size is 10000
sresult real size is 6600
Test test vmatrix vs smatrix OK!

```
