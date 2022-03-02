#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include "SDL2SoundEffects.h"

using namespace std;

struct Dice {
	int Value;
	int Count;
};

void ShowRules();
bool StartGame(int& GameMode);
void ShowDiceRoll(int DiceRemaining, int* Roll);
void PressEnter();
void DiceRoll(int DiceRemaining, int* Roll);
void EntryCounting(int DiceRemaining, int* Roll, Dice* DiceFaces);
int ScoreCounting(int& DiceRemaining, int* Roll, Dice* DiceFaces);
void CheckBoosters(int& FirstPlayerTotalScore, int& SecondPlayerTotalScore, int GM);
void OverTaking(int& FirstPlayerTotalScore, int& SecondPlayerTotalScore, int GM, int& Max);

int main() {

	setlocale(LC_ALL, "Ru");

	SDL2SoundEffects se;
	se.addSoundEffect("sounds/StartGame.mp3");    // se.playSoundEffect(0);
	se.addSoundEffect("sounds/SuccessThrow.mp3"); // se.playSoundEffect(1);
	se.addSoundEffect("sounds/FailedThrow.wav");  // se.playSoundEffect(2);
	se.addSoundEffect("sounds/SuccessTurn.wav");  // se.playSoundEffect(3);
	se.addSoundEffect("sounds/Victory.mp3");      // se.playSoundEffect(4);

	//int choice = 0;
	//while (1) {
	//	cin >> choice;
	//	se.playSoundEffect(choice);
	//}

	time_t t;
	srand((unsigned)time(&t));
	bool CyclicMenu = true; // Метка для суслик меню

	do {
		system("cls");

		int GameMode = 1; // Режим игры
		bool StartCheck = true; // Проверка начала игры
		int Roll[5] = {}; // Массив со значениями выпавших костей

		struct Dice DiceFaces[6]; // Массив структур с номиналом и количеством костей 
		DiceFaces[0].Value = 10, DiceFaces[1].Value = 2, DiceFaces[2].Value = 3;
		DiceFaces[3].Value = 4, DiceFaces[4].Value = 5, DiceFaces[5].Value = 6; // Номинал очков костей
		DiceFaces[0].Count = 0, DiceFaces[1].Count = 0, DiceFaces[2].Count = 0;
		DiceFaces[3].Count = 0, DiceFaces[4].Count = 0, DiceFaces[5].Count = 0; // Счётчик выпавших костей

		int FirstPlayerTotalScore = 0, SecondPlayerTotalScore = 0; // Общее количество очков 1-2 игроков
		int TurnScore = 0; // Количество очков, набранное за ход
		int Score = 1; // Количество очков, набранное за бросок
		int DiceRemaining = 5; // Кол-во кубиков, которые можно бросить на данный момент

		int Turn = 0; // Счетчик ходов
		int TurnSelect = 1; // Выбор, продолжить ход или окончить
		int Enter; // Переменная для удобства восприятия игрового процесса
		int Max = 0; // Переменная для реализации правила обгона

		int Ghoul = 0;

		se.playSoundEffect(0);
		StartCheck = StartGame(GameMode); // Выбор режима игры, её начало и правила

		if (!StartCheck) return 0;
		system("cls");

		while (FirstPlayerTotalScore < 1000 and SecondPlayerTotalScore < 1000) {

			Turn++, TurnSelect = 1;
			TurnScore = 0, DiceRemaining = 5;
			Enter = 0;

			if (GameMode == 1) {
				if (Turn % 2 == 1) cout << "================================ Ход игрока ================================" << endl;
				else cout << "============================== Ход компьютера ==============================" << endl;
				cout << "[Счёт] Игрок: " << FirstPlayerTotalScore << ". Компьютер: " << SecondPlayerTotalScore << endl;
			}
			else {
				if (Turn % 2 == 1) cout << "============================= Ход 1-го игрока ==============================" << endl;
				else cout << "============================= Ход 2-го игрока ==============================" << endl;
				cout << "[Счёт] Первый игрок: " << FirstPlayerTotalScore << ". Второй игрок: " << SecondPlayerTotalScore << endl;
			}

			while (TurnSelect == 1) {

				Enter++, Score = 0;
				DiceFaces[0].Count = 0;
				DiceFaces[1].Count = 0;
				DiceFaces[2].Count = 0;
				DiceFaces[3].Count = 0;
				DiceFaces[4].Count = 0;
				DiceFaces[5].Count = 0;

				if (Enter == 1 and (GameMode == 2 or (GameMode == 1 and Turn % 2 == 1))) PressEnter(); // Функция для удобства восприятия игрового процесса
				if (GameMode == 1 and Turn % 2 == 0) Sleep(2000); // Задержка для удобства восприятия игрового процесса

				DiceRoll(DiceRemaining, Roll); // Бросок костей (заполнение массива случайными числами от 1 до 6)
				ShowDiceRoll(DiceRemaining, Roll); // Вывод костей с помощью псевдографики
				EntryCounting(DiceRemaining, Roll, DiceFaces); // Подсчёт количества выпавших костей

				Score = ScoreCounting(DiceRemaining, Roll, DiceFaces); // Подсчёт количества очков, набраных за последний бросок

				if (Score > 0) se.playSoundEffect(1);

				TurnScore += Score; // Подсчёт количества очков, набранных за текущий ход
				if (Score == 0) TurnScore = 0; // Обнуление хода при нулевом броске, согласно правилам

				cout << "Количество очков за бросок: " << Score << endl;
				cout << "Количество очков за весь ход: " << TurnScore << endl;

				if (DiceRemaining == 0 and Score > 0) {
					DiceRemaining = 5;
					cout << "Все кости принесли очки. Теперь вам снова доступно 5 костей." << endl;
				}

				// Выбор компьютера, продолжать или передать ход (рандом)
				if (GameMode == 1 and Turn % 2 == 0) {
					if (DiceRemaining > 0 and Score > 0) {
						cout << "Осталось костей: " << DiceRemaining << ". Бросить еще раз? [1 - да, 2 - нет]: ";
						Sleep(2000); // Задержка для удобства восприятия игрового процесса
						TurnSelect = rand() % 2 + 1; cout << TurnSelect << endl;
						if (TurnSelect == 2) {
							cout << "Ход окончен." << endl << endl;
							se.playSoundEffect(3);
						}
					}
					else if (DiceRemaining == 0) {
						TurnSelect = 2;
						cout << "Ход окончен, т.к. у вас закончились кости." << endl << endl;
					}
					else if (Score == 0) {
						TurnSelect = 2;
						cout << "Ход окончен, т.к. вы совершили нулевой бросок." << endl;
						cout << "Все очки, набранные за этот ход, не засчитаны." << endl << endl;
						se.playSoundEffect(2);
					}
				}
				// Выбор игрока, продолжать или передать ход
				else {
					if (DiceRemaining > 0 and Score > 0) {
						do {
							if (TurnSelect < 1 or TurnSelect > 2) cout << "Неверный ввод." << endl;
							cout << "Осталось костей: " << DiceRemaining << ". Бросить еще раз? [1 - да, 2 - нет]: ";
							cin >> TurnSelect;
						} while (TurnSelect < 1 or TurnSelect > 2);
						if (TurnSelect == 2) {
							cout << "Ход окончен." << endl << endl;
							se.playSoundEffect(3);
						}
					}
					else if (DiceRemaining == 0) {
						TurnSelect = 2;
						cout << "Ход окончен, т.к. у вас закончились кости." << endl << endl;
					}
					else if (Score == 0) {
						TurnSelect = 2;
						cout << "Ход окончен, т.к. вы совершили нулевой бросок." << endl;
						cout << "Все очки, набранные за этот ход, не засчитаны." << endl << endl;
						se.playSoundEffect(2);
					}
				}
			}

			// Подсчёт общего количества очков, набранных игроками
			if (Turn % 2 == 1) FirstPlayerTotalScore += TurnScore;
			else SecondPlayerTotalScore += TurnScore;

			// Правило самосвала и такси
			CheckBoosters(FirstPlayerTotalScore, SecondPlayerTotalScore, GameMode);

			// Правило обгона
			OverTaking(FirstPlayerTotalScore, SecondPlayerTotalScore, GameMode, Max);

			if (GameMode == 1 and FirstPlayerTotalScore > 999) {
				Sleep(1000);
				cout << "==================== Игрок победил со счётом " << FirstPlayerTotalScore << " - " << SecondPlayerTotalScore << " ====================" << endl;
				se.playSoundEffect(4);
				break;
			}
			else if (GameMode == 1 and SecondPlayerTotalScore > 999) {
				Sleep(1000);
				cout << "================== Компьютер победил со счётом " << FirstPlayerTotalScore << " - " << SecondPlayerTotalScore << " ==================" << endl;
				se.playSoundEffect(4);
				Ghoul = 1;
				break;
			}
			else if (GameMode == 2 and FirstPlayerTotalScore > 999) {
				Sleep(1000);
				cout << "================= Первый игрок победил со счётом " << FirstPlayerTotalScore << " - " << SecondPlayerTotalScore << " =================" << endl;
				se.playSoundEffect(4);
				break;
			}
			else if (GameMode == 2 and SecondPlayerTotalScore > 999) {
				Sleep(1000);
				cout << "================= Второй игрок победил со счётом " << FirstPlayerTotalScore << " - " << SecondPlayerTotalScore << " =================" << endl;
				se.playSoundEffect(4);
				break;
			}

			Sleep(2000); // Задержка для удобства восприятия игрового процесса
		}

		if (Ghoul == 1) {
			cout << "Компьютер поставит паузу через 3..." << endl; Sleep(1000);
			cout << "Компьютер поставит паузу через 2..." << endl; Sleep(1000);
			cout << "Компьютер поставит паузу через 1..." << endl; Sleep(1000);
			cout << "Компьютер приостанавливает игру" << endl; Sleep(1000);
			cout << "Компьютер: ?" << endl; Sleep(1000);
			char key = 0;
			cout << "[Нажмите Enter, чтобы продолжить]" << endl;
			do {
				key = _getch();
				if (key == -32) key = _getch();
			} while (key != 13);
		}

		cout << endl << "Начать заново? [1 - начать новую игру, 0 - выйти]: "; cin >> CyclicMenu;
	} while (CyclicMenu == true);
}

void ShowRules() {
	cout << endl << "======================================================= ПРАВИЛА =======================================================" << endl;
	cout << "Игра в кости —  игра, в которой ее участники  выбрасыванием числа кости получают соответсвующее количество очков." << endl;
	cout << "В процессе игры 2 игрока по очереди бросают 5 игральных костей, комбинации которых дают очки. Цель - набрать 1000 очков." << endl;
	cout << "Номинал граней кубика:" << endl;
	cout << "1. Единица – 10 очков." << endl;
	cout << "2. Двойка – 2 очка(только в случае комбинации 3 + кубиков)." << endl;
	cout << "3. Тройка – 3 очка(только в случае комбинации 3 + кубиков)." << endl;
	cout << "4. Четверка – 4 очка(только в случае комбинации 3 + кубиков)." << endl;
	cout << "5. Пятерка – 5 очков." << endl;
	cout << "6. Шестерка – 6 очков(только в случае комбинации 3 + кубиков)." << endl;
	cout << "Единица и пятерка приносят свои номинальные очки, даже если встречаются единожды в комбинации (дважды – номинал*2 )." << endl;
	cout << "Остальные грани приносят очки только в случае комбинации 3+ кубиков с одинаковым номиналом." << endl;
	cout << "Комбинация из 3 кубиков с одинаковой гранью умножает номинал в 10 раз, из 4 – в 20 раз, из 5 – в 100 раз." << endl;
	cout << "Комбинация 1-2-3-4-5 приносит 125 очков, а 2-3-4-5-6 – 250 очков." << endl;
	cout << "Если бросок оказался результативным (принес очки),то игрок откладывает кубики, принесшие очки," << endl;
	cout << "и может снова бросить оставшиеся (если в комбинациях участвовали все 5 кубиков, остаются доступными все)." << endl;
	cout << "Но, если в следующем броске игрок получает 0 очков, то очки за весь ход сгорают, и бросок переходит к другому игроку." << endl;
	cout << "* Правило обгона: если игрок \"обгоняет\" другого, т.е. до хода он имел меньшую сумму очков, а после хода большую," << endl;
	cout << "(но не равную, при которой обгон не засчитывается!), то обгоняющий получает, а обгоняемый теряет 50 очков." << endl;
	cout << "* Такси: если игрок набирает 100, 200, 300 и т.д. очков, то он попадает на \"такси\" и ему добавляется 175 очков." << endl;
	cout << "* Самосвал: если игрок набирает 150, 250, 350 и т.д. очков, то он попадает на \"самосвал\" и с него списывается 175 очков." << endl;
	cout << "======================================================= ПРАВИЛА =======================================================" << endl << endl;
}
bool StartGame(int& GameMode) {

	bool StartGameSelect = true;
	int StartGameSelect2 = 1;

	cout << "Добро пожаловать в консольную \"Игру в кости\"" << endl;

	do {
		if (StartGameSelect2 < 1 or StartGameSelect2 > 3) cout << "Неверный ввод!" << endl;
		cout << "[Для начала игры - введите 1, для просмотра правил - 2, для выхода из игры - 3]: "; cin >> StartGameSelect2;
	} while (StartGameSelect2 < 1 or StartGameSelect2 > 3);

	if (StartGameSelect2 == 3) StartGameSelect = false;

	if (StartGameSelect2 == 2) {

		ShowRules();

		do {
			if (StartGameSelect2 < 1 or StartGameSelect2 > 2) cout << "Неверный ввод!" << endl;
			cout << "Начать игру? [1 - да, 2 - выход]: "; cin >> StartGameSelect2;
		} while (StartGameSelect2 < 1 or StartGameSelect2 > 2);

		if (StartGameSelect2 == 2) StartGameSelect = false;
	}

	if (!StartGameSelect) return 0;

	if (StartGameSelect) {
		do {
			if (GameMode < 1 or GameMode > 2) cout << "Неверный ввод!" << endl;
			cout << "Выберите режим игры [1 - против компьютера, 2 - два игрока]: "; cin >> GameMode;
		} while (GameMode < 1 or GameMode > 2);
	}

	return StartGameSelect;
}
void PressEnter() {
	char key = 0;
	cout << "[Нажмите Enter для броска костей]" << endl;
	do {
		key = _getch();
		if (key == -32) key = _getch();
	} while (key != 13);
}
void DiceRoll(int DiceRemaining, int* Roll) {
	for (int i = 0; i < DiceRemaining; i++) {
		Roll[i] = rand() % 6 + 1;
	}
}
void ShowDiceRoll(int DiceRemaining, int* Roll) {

	cout << endl << "Комбинация кубиков: " << endl;

	for (int j = 0; j < DiceRemaining; j++) {
		cout << " ___________  ";
	}
	cout << endl;
	for (int j = 0; j < DiceRemaining; j++) {
		cout << "|           | ";
	}
	cout << endl;

	for (int j = 0; j < DiceRemaining; j++) {
		switch (Roll[j]) {
		case 1:
			cout << "|           | "; break;
		case 2:
			cout << "|           | "; break;
		case 3:
			cout << "|  O        | "; break;
		case 4:
			cout << "|  O     O  | "; break;
		case 5:
			cout << "|  O     O  | "; break;
		case 6:
			cout << "|  O  O  O  | "; break;
		}
	}
	cout << endl;
	for (int j = 0; j < DiceRemaining; j++) {
		switch (Roll[j]) {
		case 1:
			cout << "|     O     | "; break;
		case 2:
			cout << "|  O     O  | "; break;
		case 3:
			cout << "|     O     | "; break;
		case 4:
			cout << "|           | "; break;
		case 5:
			cout << "|     O     | "; break;
		case 6:
			cout << "|  O  O  O  | "; break;
		}
	}
	cout << endl;
	for (int j = 0; j < DiceRemaining; j++) {
		switch (Roll[j]) {
		case 1:
			cout << "|           | "; break;
		case 2:
			cout << "|           | "; break;
		case 3:
			cout << "|        O  | "; break;
		case 4:
			cout << "|  O     O  | "; break;
		case 5:
			cout << "|  O     O  | "; break;
		case 6:
			cout << "|  O  O  O  | "; break;
		}
	}
	cout << endl;
	for (int j = 0; j < DiceRemaining; j++) {
		cout << "|___________| ";
	}
	cout << endl;
}
void EntryCounting(int DiceRemaining, int* Roll, Dice* DiceFaces) {
	for (int i = 0; i < DiceRemaining; i++) {
		switch (Roll[i]) {
		case 1: { DiceFaces[0].Count++; break; }
		case 2: { DiceFaces[1].Count++; break; }
		case 3: { DiceFaces[2].Count++; break; }
		case 4: { DiceFaces[3].Count++; break; }
		case 5: { DiceFaces[4].Count++; break; }
		case 6: { DiceFaces[5].Count++; break; }
		default: break;
		}
	}
}
int ScoreCounting(int& DiceRemaining, int* Roll, Dice* DiceFaces) {

	int Score = 0;

	if (DiceFaces[0].Count == 1 and DiceFaces[1].Count == 1 and DiceFaces[2].Count == 1 and DiceFaces[3].Count == 1 and DiceFaces[4].Count == 1) Score += 125;
	else if (DiceFaces[1].Count == 1 and DiceFaces[2].Count == 1 and DiceFaces[3].Count == 1 and DiceFaces[4].Count == 1 and DiceFaces[5].Count == 1) Score += 250;
	else {
		if (DiceFaces[0].Count < 3) { Score += DiceFaces[0].Value * DiceFaces[0].Count; DiceRemaining -= DiceFaces[0].Count; }
		if (DiceFaces[4].Count < 3) { Score += DiceFaces[4].Value * DiceFaces[4].Count; DiceRemaining -= DiceFaces[4].Count; }
	}

	if (DiceFaces[0].Count > 2) {
		switch (DiceFaces[0].Count) {
		case 3: { Score += DiceFaces[0].Value * 10; DiceRemaining -= 3; break; }
		case 4: { Score += DiceFaces[0].Value * 20; DiceRemaining -= 4; break; }
		case 5: { Score += DiceFaces[0].Value * 100; break; }
		default: break;
		}
	}
	else if (DiceFaces[1].Count > 2) {
		switch (DiceFaces[1].Count) {
		case 3: { Score += DiceFaces[1].Value * 10; DiceRemaining -= 3; break; }
		case 4: { Score += DiceFaces[1].Value * 20; DiceRemaining -= 4; break; }
		case 5: { Score += DiceFaces[1].Value * 100; break; }
		default: break;
		}
	}
	else if (DiceFaces[2].Count > 2) {
		switch (DiceFaces[2].Count) {
		case 3: { Score += DiceFaces[2].Value * 10; DiceRemaining -= 3; break; }
		case 4: { Score += DiceFaces[2].Value * 20; DiceRemaining -= 4; break; }
		case 5: { Score += DiceFaces[2].Value * 100; break; }
		default: break;
		}
	}
	else if (DiceFaces[3].Count > 2) {
		switch (DiceFaces[3].Count) {
		case 3: { Score += DiceFaces[3].Value * 10; DiceRemaining -= 3; break; }
		case 4: { Score += DiceFaces[3].Value * 20; DiceRemaining -= 4; break; }
		case 5: { Score += DiceFaces[3].Value * 100; break; }
		default: break;
		}
	}
	else if (DiceFaces[4].Count > 2) {
		switch (DiceFaces[4].Count) {
		case 3: { Score += DiceFaces[4].Value * 10; DiceRemaining -= 3; break; }
		case 4: { Score += DiceFaces[4].Value * 20; DiceRemaining -= 4; break; }
		case 5: { Score += DiceFaces[4].Value * 100; break; }
		default: break;
		}
	}
	else if (DiceFaces[5].Count > 2) {
		switch (DiceFaces[5].Count) {
		case 3: { Score += DiceFaces[5].Value * 10; DiceRemaining -= 3; break; }
		case 4: { Score += DiceFaces[5].Value * 20; DiceRemaining -= 4; break; }
		case 5: { Score += DiceFaces[5].Value * 100; break; }
		default: break;
		}
	}

	return Score;
}
void CheckBoosters(int& FirstPlayerTotalScore, int& SecondPlayerTotalScore, int GM) {
	if (GM == 1) {
		if (FirstPlayerTotalScore % 100 == 0 and FirstPlayerTotalScore != 0) {
			FirstPlayerTotalScore += 175;
			cout << "[Такси] Игрок получает 175 очков!" << endl << endl;
		}
		if (FirstPlayerTotalScore % 100 == 50 and FirstPlayerTotalScore != 0) {
			FirstPlayerTotalScore -= 175;
			cout << "[Самосвал] Игрок теряет 175 очков!" << endl << endl;
		}
		if (SecondPlayerTotalScore % 100 == 0 and SecondPlayerTotalScore != 0) {
			SecondPlayerTotalScore += 175;
			cout << "[Такси] Компьютер получает 175 очков!" << endl << endl;
		}
		if (SecondPlayerTotalScore % 100 == 50 and SecondPlayerTotalScore != 0) {
			SecondPlayerTotalScore -= 175;
			cout << "[Самосвал] Компьютер теряет 175 очков!" << endl << endl;
		}
	}
	else {
		if (FirstPlayerTotalScore % 100 == 0 and FirstPlayerTotalScore != 0) {
			FirstPlayerTotalScore += 175;
			cout << "[Такси] Первый игрок получает 175 очков!" << endl << endl;
		}
		if (FirstPlayerTotalScore % 100 == 50 and FirstPlayerTotalScore != 0) {
			FirstPlayerTotalScore -= 175;
			cout << "[Самосвал] Первый игрок теряет 175 очков!" << endl << endl;
		}
		if (SecondPlayerTotalScore % 100 == 0 and SecondPlayerTotalScore != 0) {
			SecondPlayerTotalScore += 175;
			cout << "[Такси] Второй игрок получает 175 очков!" << endl << endl;
		}
		if (SecondPlayerTotalScore % 100 == 50 and SecondPlayerTotalScore != 0) {
			SecondPlayerTotalScore -= 175;
			cout << "[Самосвал] Второй игрок теряет 175 очков!" << endl << endl;
		}
	}

	if (FirstPlayerTotalScore < 0) FirstPlayerTotalScore = 0;
	if (SecondPlayerTotalScore < 0) SecondPlayerTotalScore = 0;
}
void OverTaking(int& FirstPlayerTotalScore, int& SecondPlayerTotalScore, int GM, int& Max) {
	if (GM == 1) {
		if (Max == 0 and FirstPlayerTotalScore > SecondPlayerTotalScore) {
			Max = 1;
		}
		else if (Max == 0 and SecondPlayerTotalScore > FirstPlayerTotalScore) {
			Max = 2;
		}
		if (Max == 1 and SecondPlayerTotalScore > FirstPlayerTotalScore) {
			FirstPlayerTotalScore -= 50;
			SecondPlayerTotalScore += 50;
			Max = 2;
			cout << "[Обгон] Компьютер обогнал игрока." << endl;
			cout << "[Обгон] Компьютер получает 50 очков." << endl;
			cout << "[Обгон] Игрок теряет 50 очков." << endl << endl;
		}
		else if (Max == 2 and FirstPlayerTotalScore > SecondPlayerTotalScore) {
			FirstPlayerTotalScore += 50;
			SecondPlayerTotalScore -= 50;
			Max = 1;
			cout << "[Обгон] Игрок обогнал компьютер." << endl;
			cout << "[Обгон] Игрок получает 50 очков." << endl;
			cout << "[Обгон] Компьютер теряет 50 очков." << endl << endl;
		}
	}
	else {
		if (Max == 0 and FirstPlayerTotalScore > SecondPlayerTotalScore) {
			Max = 1;
		}
		else if (Max == 0 and SecondPlayerTotalScore > FirstPlayerTotalScore) {
			Max = 2;
		}
		if (Max == 1 and SecondPlayerTotalScore > FirstPlayerTotalScore) {
			FirstPlayerTotalScore -= 50;
			SecondPlayerTotalScore += 50;
			Max = 2;
			cout << "[Обгон] Второй игрок обогнал первого." << endl;
			cout << "[Обгон] Второй игрок получает 50 очков." << endl;
			cout << "[Обгон] Первый игрок теряет 50 очков." << endl << endl;
		}
		else if (Max == 2 and FirstPlayerTotalScore > SecondPlayerTotalScore) {
			FirstPlayerTotalScore += 50;
			SecondPlayerTotalScore -= 50;
			Max = 1;
			cout << "[Обгон] Первый игрок обогнал второго." << endl;
			cout << "[Обгон] Первый игрок получает 50 очков." << endl;
			cout << "[Обгон] Второй игрок теряет 50 очков." << endl << endl;
		}
	}

	if (FirstPlayerTotalScore < 0) FirstPlayerTotalScore = 0;
	if (SecondPlayerTotalScore < 0) SecondPlayerTotalScore = 0;
	if (FirstPlayerTotalScore == 0 and SecondPlayerTotalScore == 0) Max = 0;
}

//  ___________   ___________   ___________   ___________   ___________   ___________
// |           | |           | |           | |           | |           | |           |
// |  O  O  O  | |  O     O  | |  O     O  | |  O        | |           | |           |
// |  O  O  O  | |     O     | |           | |     O     | |  O     O  | |     O     |
// |  O  O  O  | |  O     O  | |  O     O  | |        O  | |           | |           |
// |___________| |___________| |___________| |___________| |___________| |___________|