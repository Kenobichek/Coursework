#include <iostream>
#include <ctime> 
#include <queue>
#include <list>
#include <windows.h>

using namespace std;

void freeing_up_memory(int n, int** data) {										// Освобождение памяти
	for (int i = 0; i < n; i++) {
		delete[] data[i];
	}
	delete[] data;
}

int input(int& n, int& m) {														// функция изначального ввода значений 
	cout << "************************************************************\n";
	cout << "********************< Добро пожаловать >********************\n";
	cout << "*************< Вы находитесь в начальном меню  >************\n\n";

	cout << "Введите количество городов:\n";
	cin >> n;

	cout << "\nВведите количество путей:\n";
	cin >> m;

	cout << "\nВручную выбрать дороги: 1\nСоздать дороги случайно: 2\n";
	int opt;
	do {
		cin >> opt;
	} while (opt != 1 && opt != 2);

	return opt == 1 ? 1 : 2;
}

void arrayPreparation(int** arr, int n) {										// функция заполнения матрицы значениями -1 
	for (int i = 0; i < n; i++) {
		arr[i] = new int[n];
		for (int j = 0; j < n; j++) {
			arr[i][j] = -1;
		}
	}
}

void createPersonally(int n, int m, int** data) {								// функция заполнения персонально матрицы смежности
	for (int i = 0; i < m; i++) {
		int x, y, dist;

		while (1) {
			cout << "\nСоздать дорогу от: ";
			cin >> x;
			cout << "До: ";
			cin >> y;
			cout << "Расстояние: ";
			cin >> dist;

			if (x <= n && y <= n && data[x - 1][y - 1] == -1) {
				data[x - 1][y - 1] = dist;
				data[y - 1][x - 1] = dist;
				cout << "\n--------------------\n";
				cout << "Дорога добавлена :) \n";
				cout << "--------------------\n\n";
				break;
			}

			if (x > n || y > n) {
				cout << "\n-----------------------------------\n";
				cout << "Одного из городов не существует :( \n";
				cout << "Пожалуйста перепишите... \n";
				cout << "-----------------------------------\n\n";
				continue;
			}

			if (data[x - 1][y - 1] != -1) {
				cout << "Дорога между городами уже есть, заменить ?(y/n) \n";
				char otv;
				cin >> otv;

				if (otv == 'y') {
					data[x - 1][y - 1] = dist;
					data[y - 1][x - 1] = dist;
					cout << "\n--------------------\n";
					cout << "Дорога изменена :) \n";
					cout << "--------------------\n\n";
					break;
				}
				else {
					continue;
				}
			}
		}
	}
}

void createRandom(int n, int m, int** data) {									// функция случайного заполнения матрицы смежности
	for (int i = 0; i < m; i++) {
		int x, y, dist;
		while (1) {
			x = rand() % n;
			y = rand() % n;
			dist = (rand() % 10) + 1;
			if (x < n && y < n && y != x && data[x][y] == -1) {
				break;
			}
		}
		data[x][y] = dist;
		data[y][x] = dist;
	}
}

int Dijkstra_Algorithm(int n, int a, int b, int** data) {						// функция для нахождения кратчайшого пути из а в б;
	queue <int> vertex_queue;													// очередь для хранения всех вершин какие мы должны обойти 
	int* dist = new int[n];														// массив дистанций от вершини а

	vertex_queue.push(a);

	for (int i = 0; i < n; i++) {
		dist[i] = -1;															// заполнение массива дистанций значениями -1
	}

	dist[a] = 0;																// почначальная дистанция

	while (!vertex_queue.empty()) {
		int ver = vertex_queue.front();
		vertex_queue.pop();

		for (int i = 0; i < n; i++) {
			if (data[ver][i] != -1 && (dist[i] == -1 || dist[i] > dist[ver] + data[ver][i])) {
				dist[i] = dist[ver] + data[ver][i];
				vertex_queue.push(i);
			}
		}
	}

	return dist[b];
}

void findAllWays(int n, int ver, int b, int** data, int*& relations, list <int>* way) {		// функция перебора всех путей от вершины а до б 
	way->push_back(ver);
	relations[ver] = 1;
	if (ver == b) {

		for (auto iter = way->begin(); iter != way->end(); iter++)
		{
			cout << *iter + 1;

			if (iter != way->end()) {
				iter++;
				if (iter != way->end()) {
					cout << " -> ";
				}
				iter--;
			}
		}

		cout << endl;
		return;
	}

	for (int i = 0; i < n; i++) {

		if (data[ver][i] != -1 && relations[i] != 1) {
			findAllWays(n, i, b, data, relations, way);
			way->pop_back();
			relations[i] = 0;
		}
	}
}

void paths_a_to_b(int n, int a, int b, int** data) {			// функция нахождения всех возможных путей из а в б
	int* relations = new int[n];							// связи
	list <int> way;
	findAllWays(n, a, b, data, relations, &way);
	cout << endl;
}

void DFS(int n, int ver, int** data, int*& relations) {		// поиск в глубину
	relations[ver] = 1;
	for (int i = 0; i < n; i++) {
		if (data[ver][i] != -1 && relations[i] != 1) {
			DFS(n, i, data, relations);
		}
	}
}

void Dijkstra_Algorithm2(int n, int a, int** data) {		// функция для нахождения кратчайшего пути из а в любую вершину 
	queue <int> vertex_queue;								// очередь для хранения всех вершин какие мы должны обойти 
	int* dist = new int[n];									// массив дистанций от вершини а

	vertex_queue.push(a);

	for (int i = 0; i < n; i++) {
		dist[i] = -1;										// заполнение массива значениями -1
	}

	dist[a] = 0;											// почначальная дистанция

	while (!vertex_queue.empty()) {
		int ver = vertex_queue.front();
		vertex_queue.pop();

		for (int i = 0; i < n; i++) {
			if (data[ver][i] != -1 && (dist[i] == -1 || dist[i] > dist[ver] + data[ver][i])) {
				dist[i] = dist[ver] + data[ver][i];
				vertex_queue.push(i);
			}
		}
	}

	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << "от вершины " << a + 1 << " до вершины " << i + 1 << " = " << dist[i] << endl;
	}
	cout << endl;
}

void unrelated_cities(int n, int** data) {					// функция для нахождения всех вершин не связанных между собой
	int** relations = new int* [n];							// связи
	arrayPreparation(relations, n);

	for (int i = 0; i < n; i++) {
		DFS(n, i, data, relations[i]);
	}

	cout << endl;

	for (int i = 0; i < n; i++) {
		cout << "вершина " << i + 1 << " не имеет дорог к: ";
		for (int j = 0; j < n; j++) {
			if (relations[i][j] == -1 && i != j) {
				cout << j + 1 << " ";
			}
		}
		cout << endl;
	}

	cout << endl;
}

void print(int n, int** data) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << data[i][j] << " ";
		}
		cout << endl;
	}
}

void change_number_vertices(int& n, int**& data) {			// Изменить количество вершин
	cout << "Введите сколько вершин должно быть в графе: \n";
	int newN;
	cin >> newN;
	int** newData = new int* [newN];
	arrayPreparation(newData, newN);
	int tmp = min(n, newN);
	for (int i = 0; i < tmp; i++) {
		for (int j = 0; j < tmp; j++) {
			newData[i][j] = data[i][j];
		}
	}
	swap(data, newData);
	freeing_up_memory(n, newData);
	n = newN;
	cout << endl;
}

void change_number_ribs(int n, int& m, int** data) {			// Изменить количество ребер
	cout << "1. Добавить ребро\n2. Удалить ребро\n\n";
	int opt;
	do {
		cin >> opt;
	} while (opt != 1 && opt != 2);

	if (opt == 1) {
		int x, y, dist;
		while (1) {
			cout << "\nСоздать дорогу от: ";
			cin >> x;
			cout << "До: ";
			cin >> y;
			cout << "Расстояние: ";
			cin >> dist;

			if (x <= n && y <= n && data[x - 1][y - 1] == -1) {
				data[x - 1][y - 1] = dist;
				data[y - 1][x - 1] = dist;
				cout << "\n--------------------\n";
				cout << "Дорога добавлена :) \n";
				cout << "--------------------\n\n";
				break;
			}

			if (x > n || y > n) {
				cout << "\n-----------------------------------\n";
				cout << "Одного из городов не существует :( \n";
				cout << "Пожалуйста перепишите... \n";
				cout << "-----------------------------------\n\n";
				continue;
			}

			if (data[x - 1][y - 1] != -1) {
				cout << "Дорога между городами уже есть, заменить ?(y/n) \n";
				char otv;
				cin >> otv;

				if (otv == 'y') {
					data[x - 1][y - 1] = dist;
					data[y - 1][x - 1] = dist;
					cout << "\n--------------------\n";
					cout << "Дорога изменена :) \n";
					cout << "--------------------\n\n";
					break;
				}
				else {
					break;
				}
			}
		}
	}
	else {
		int x, y;
		cout << "\nУдалить дорогу от: ";
		cin >> x;
		cout << "До: ";
		cin >> y;
		if (data[x - 1][y - 1] == -1) {
			cout << "\n--------------------------------\n";
			cout << "Нельзя удалить дорогу, какой нет\n";
			cout << "----------------------------------\n\n";
		}
		else {
			data[x - 1][y - 1] = -1;
			data[y - 1][x - 1] = -1;
			cout << "\n--------------------------------------------------\n";
			cout << "Дорога от " << x << " до " << y << " была удалена \n";
			cout << "--------------------------------------------------\n\n";
		}
	}
}

int menu(int n, int m, int** data) {		// функция меню
	system("cls");

	cout << "------------МЕНЮ------------\n";
	cout << "Дано:\n";
	cout << "Количество городов: " << n << "\n";
	cout << "Количество дорог: " << m << "\n\n";
	cout << "Определить:\n";
	cout << "1. Кратчайший путь из города а в город б\n";
	cout << "2. Все возможные пути из города а в город б\n";
	cout << "3. Минимальные расстояния от города а до любого из городов\n";
	cout << "4. Все города, не связанные между собой дорогами\n\n";
	cout << "Дополнительные функции:\n";
	cout << "5. Показать матрицу смежности\n";
	cout << "6. Изменить количество ребер\n";
	cout << "7. Изменить количество вершин\n";
	cout << "8. Выход\n\n";
	cout << "Выберите пункт из меню: \n";

	int item;
	cin >> item;
	switch (item) {							// выбираем пункт из меню
	case 1: {
		system("cls");
		cout << "Дано:\n";
		cout << "Количество городов: " << n << "\n";
		cout << "Количество дорог: " << m << "\n\n";
		cout << "Кратчайший путь из а в б\n";
		int a, b;
		while (1) {
			cout << "город а: ";
			cin >> a;
			cout << "\nгород б: ";
			cin >> b;
			if (a <= n && b <= n) {
				break;
			}
			cout << "Что-то пошло не так. Пожалуйста, попробуйте еще раз.\n";
		}

		cout << "\nКратчайший путь из города " << a << " в город " << b << " = " << Dijkstra_Algorithm(n, a - 1, b - 1, data) << "\n\n";
		system("pause");
		system("cls");
		menu(n, m, data);
		break;
	}
	case 2: {
		system("cls");
		cout << "Дано:\n";
		cout << "Количество городов: " << n << "\n";
		cout << "Количество дорог: " << m << "\n\n";
		cout << "\n";
		int a, b;
		while (1) {
			cout << "город а: ";
			cin >> a;
			cout << "\nгород б: ";
			cin >> b;

			if (a <= n && b <= n) {
				break;
			}
			cout << "Что-то пошло не так. Пожалуйста, попробуйте еще раз.\n";
		}
		cout << "\nВсе возможные пути из а в б\n\n";
		paths_a_to_b(n, a - 1, b - 1, data);
		system("pause");
		system("cls");
		menu(n, m, data);
		break;
	}
	case 3: {
		system("cls");
		cout << "Дано:\n";
		cout << "Количество городов: " << n << "\n";
		cout << "Количество дорог: " << m << "\n\n";
		cout << "Кратчайший путь из а до всех остальных вершин\n";
		int a;
		while (1) {
			cout << "город а: ";
			cin >> a;
			if (a <= n) {
				break;
			}
			cout << "Что-то пошло не так. Пожалуйста, попробуйте еще раз.\n";
		}
		Dijkstra_Algorithm2(n, a - 1, data);
		system("pause");
		system("cls");
		menu(n, m, data);
		break;
	}
	case 4: {
		system("cls");
		cout << "Дано:\n";
		cout << "Количество городов: " << n << "\n";
		cout << "Количество дорог: " << m << "\n\n";
		cout << "Все города, не связанные между собой дорогами\n";

		unrelated_cities(n, data);
		system("pause");
		system("cls");
		menu(n, m, data);
		break;
	}
	case 5: {
		system("cls");
		cout << "Дано:\n";
		cout << "Количество городов: " << n << "\n";
		cout << "Количество дорог: " << m << "\n\n";
		cout << "МАТРИЦА СМЕЖНОСТИ:\n";

		print(n, data);

		cout << "\n-1 - нет дороги";
		cout << "\nиначе просто расстояние от города i до j\n\n";
		system("pause");
		system("cls");
		menu(n, m, data);
		break;
	}
	case 6: {
		system("cls");
		change_number_ribs(n, m, data);
		system("pause");
		system("cls");
		menu(n, m, data);
		break;
	}
	case 7: {
		system("cls");
		change_number_vertices(n, data);
		system("pause");
		system("cls");
		menu(n, m, data);
		break;
	}
	case 8: {
		system("cls");
		cout << "****************** < До свидания > ******************\n";
		cout << "***** < Cпасибо что использовали эту программу > *****\n";
		freeing_up_memory(n, data);
		return 0;
		break;
	}
	default: {
		system("cls");
		cout << "Что-то пошло не так. Пожалуйста, попробуйте еще раз.\n\n";
		system("pause");
		system("cls");
		menu(n, m, data);
		break;
	}
	}
}

int main() {

	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	int n, m;					// количество городов, количество путей

	int fill = input(n, m);

	int** data = new int* [n];

	arrayPreparation(data, n);

	fill == 1 ? createPersonally(n, m, data) : createRandom(n, m, data);

	menu(n, m, data);

	return 0;
}