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
	bool CyclicMenu = true; // ����� ��� ������ ����

	do {
		system("cls");

		int GameMode = 1; // ����� ����
		bool StartCheck = true; // �������� ������ ����
		int Roll[5] = {}; // ������ �� ���������� �������� ������

		struct Dice DiceFaces[6]; // ������ �������� � ��������� � ����������� ������ 
		DiceFaces[0].Value = 10, DiceFaces[1].Value = 2, DiceFaces[2].Value = 3;
		DiceFaces[3].Value = 4, DiceFaces[4].Value = 5, DiceFaces[5].Value = 6; // ������� ����� ������
		DiceFaces[0].Count = 0, DiceFaces[1].Count = 0, DiceFaces[2].Count = 0;
		DiceFaces[3].Count = 0, DiceFaces[4].Count = 0, DiceFaces[5].Count = 0; // ������� �������� ������

		int FirstPlayerTotalScore = 0, SecondPlayerTotalScore = 0; // ����� ���������� ����� 1-2 �������
		int TurnScore = 0; // ���������� �����, ��������� �� ���
		int Score = 1; // ���������� �����, ��������� �� ������
		int DiceRemaining = 5; // ���-�� �������, ������� ����� ������� �� ������ ������

		int Turn = 0; // ������� �����
		int TurnSelect = 1; // �����, ���������� ��� ��� ��������
		int Enter; // ���������� ��� �������� ���������� �������� ��������
		int Max = 0; // ���������� ��� ���������� ������� ������

		int Ghoul = 0;

		se.playSoundEffect(0);
		StartCheck = StartGame(GameMode); // ����� ������ ����, � ������ � �������

		if (!StartCheck) return 0;
		system("cls");

		while (FirstPlayerTotalScore < 1000 and SecondPlayerTotalScore < 1000) {

			Turn++, TurnSelect = 1;
			TurnScore = 0, DiceRemaining = 5;
			Enter = 0;

			if (GameMode == 1) {
				if (Turn % 2 == 1) cout << "================================ ��� ������ ================================" << endl;
				else cout << "============================== ��� ���������� ==============================" << endl;
				cout << "[����] �����: " << FirstPlayerTotalScore << ". ���������: " << SecondPlayerTotalScore << endl;
			}
			else {
				if (Turn % 2 == 1) cout << "============================= ��� 1-�� ������ ==============================" << endl;
				else cout << "============================= ��� 2-�� ������ ==============================" << endl;
				cout << "[����] ������ �����: " << FirstPlayerTotalScore << ". ������ �����: " << SecondPlayerTotalScore << endl;
			}

			while (TurnSelect == 1) {

				Enter++, Score = 0;
				DiceFaces[0].Count = 0;
				DiceFaces[1].Count = 0;
				DiceFaces[2].Count = 0;
				DiceFaces[3].Count = 0;
				DiceFaces[4].Count = 0;
				DiceFaces[5].Count = 0;

				if (Enter == 1 and (GameMode == 2 or (GameMode == 1 and Turn % 2 == 1))) PressEnter(); // ������� ��� �������� ���������� �������� ��������
				if (GameMode == 1 and Turn % 2 == 0) Sleep(2000); // �������� ��� �������� ���������� �������� ��������

				DiceRoll(DiceRemaining, Roll); // ������ ������ (���������� ������� ���������� ������� �� 1 �� 6)
				ShowDiceRoll(DiceRemaining, Roll); // ����� ������ � ������� �������������
				EntryCounting(DiceRemaining, Roll, DiceFaces); // ������� ���������� �������� ������

				Score = ScoreCounting(DiceRemaining, Roll, DiceFaces); // ������� ���������� �����, �������� �� ��������� ������

				if (Score > 0) se.playSoundEffect(1);

				TurnScore += Score; // ������� ���������� �����, ��������� �� ������� ���
				if (Score == 0) TurnScore = 0; // ��������� ���� ��� ������� ������, �������� ��������

				cout << "���������� ����� �� ������: " << Score << endl;
				cout << "���������� ����� �� ���� ���: " << TurnScore << endl;

				if (DiceRemaining == 0 and Score > 0) {
					DiceRemaining = 5;
					cout << "��� ����� �������� ����. ������ ��� ����� �������� 5 ������." << endl;
				}

				// ����� ����������, ���������� ��� �������� ��� (������)
				if (GameMode == 1 and Turn % 2 == 0) {
					if (DiceRemaining > 0 and Score > 0) {
						cout << "�������� ������: " << DiceRemaining << ". ������� ��� ���? [1 - ��, 2 - ���]: ";
						Sleep(2000); // �������� ��� �������� ���������� �������� ��������
						TurnSelect = rand() % 2 + 1; cout << TurnSelect << endl;
						if (TurnSelect == 2) {
							cout << "��� �������." << endl << endl;
							se.playSoundEffect(3);
						}
					}
					else if (DiceRemaining == 0) {
						TurnSelect = 2;
						cout << "��� �������, �.�. � ��� ����������� �����." << endl << endl;
					}
					else if (Score == 0) {
						TurnSelect = 2;
						cout << "��� �������, �.�. �� ��������� ������� ������." << endl;
						cout << "��� ����, ��������� �� ���� ���, �� ���������." << endl << endl;
						se.playSoundEffect(2);
					}
				}
				// ����� ������, ���������� ��� �������� ���
				else {
					if (DiceRemaining > 0 and Score > 0) {
						do {
							if (TurnSelect < 1 or TurnSelect > 2) cout << "�������� ����." << endl;
							cout << "�������� ������: " << DiceRemaining << ". ������� ��� ���? [1 - ��, 2 - ���]: ";
							cin >> TurnSelect;
						} while (TurnSelect < 1 or TurnSelect > 2);
						if (TurnSelect == 2) {
							cout << "��� �������." << endl << endl;
							se.playSoundEffect(3);
						}
					}
					else if (DiceRemaining == 0) {
						TurnSelect = 2;
						cout << "��� �������, �.�. � ��� ����������� �����." << endl << endl;
					}
					else if (Score == 0) {
						TurnSelect = 2;
						cout << "��� �������, �.�. �� ��������� ������� ������." << endl;
						cout << "��� ����, ��������� �� ���� ���, �� ���������." << endl << endl;
						se.playSoundEffect(2);
					}
				}
			}

			// ������� ������ ���������� �����, ��������� ��������
			if (Turn % 2 == 1) FirstPlayerTotalScore += TurnScore;
			else SecondPlayerTotalScore += TurnScore;

			// ������� ��������� � �����
			CheckBoosters(FirstPlayerTotalScore, SecondPlayerTotalScore, GameMode);

			// ������� ������
			OverTaking(FirstPlayerTotalScore, SecondPlayerTotalScore, GameMode, Max);

			if (GameMode == 1 and FirstPlayerTotalScore > 999) {
				Sleep(1000);
				cout << "==================== ����� ������� �� ������ " << FirstPlayerTotalScore << " - " << SecondPlayerTotalScore << " ====================" << endl;
				se.playSoundEffect(4);
				break;
			}
			else if (GameMode == 1 and SecondPlayerTotalScore > 999) {
				Sleep(1000);
				cout << "================== ��������� ������� �� ������ " << FirstPlayerTotalScore << " - " << SecondPlayerTotalScore << " ==================" << endl;
				se.playSoundEffect(4);
				Ghoul = 1;
				break;
			}
			else if (GameMode == 2 and FirstPlayerTotalScore > 999) {
				Sleep(1000);
				cout << "================= ������ ����� ������� �� ������ " << FirstPlayerTotalScore << " - " << SecondPlayerTotalScore << " =================" << endl;
				se.playSoundEffect(4);
				break;
			}
			else if (GameMode == 2 and SecondPlayerTotalScore > 999) {
				Sleep(1000);
				cout << "================= ������ ����� ������� �� ������ " << FirstPlayerTotalScore << " - " << SecondPlayerTotalScore << " =================" << endl;
				se.playSoundEffect(4);
				break;
			}

			Sleep(2000); // �������� ��� �������� ���������� �������� ��������
		}

		if (Ghoul == 1) {
			cout << "��������� �������� ����� ����� 3..." << endl; Sleep(1000);
			cout << "��������� �������� ����� ����� 2..." << endl; Sleep(1000);
			cout << "��������� �������� ����� ����� 1..." << endl; Sleep(1000);
			cout << "��������� ���������������� ����" << endl; Sleep(1000);
			cout << "���������: ?" << endl; Sleep(1000);
			char key = 0;
			cout << "[������� Enter, ����� ����������]" << endl;
			do {
				key = _getch();
				if (key == -32) key = _getch();
			} while (key != 13);
		}

		cout << endl << "������ ������? [1 - ������ ����� ����, 0 - �����]: "; cin >> CyclicMenu;
	} while (CyclicMenu == true);
}

void ShowRules() {
	cout << endl << "======================================================= ������� =======================================================" << endl;
	cout << "���� � ����� �  ����, � ������� �� ���������  ������������� ����� ����� �������� �������������� ���������� �����." << endl;
	cout << "� �������� ���� 2 ������ �� ������� ������� 5 ��������� ������, ���������� ������� ���� ����. ���� - ������� 1000 �����." << endl;
	cout << "������� ������ ������:" << endl;
	cout << "1. ������� � 10 �����." << endl;
	cout << "2. ������ � 2 ����(������ � ������ ���������� 3 + �������)." << endl;
	cout << "3. ������ � 3 ����(������ � ������ ���������� 3 + �������)." << endl;
	cout << "4. �������� � 4 ����(������ � ������ ���������� 3 + �������)." << endl;
	cout << "5. ������� � 5 �����." << endl;
	cout << "6. �������� � 6 �����(������ � ������ ���������� 3 + �������)." << endl;
	cout << "������� � ������� �������� ���� ����������� ����, ���� ���� ����������� �������� � ���������� (������ � �������*2 )." << endl;
	cout << "��������� ����� �������� ���� ������ � ������ ���������� 3+ ������� � ���������� ���������." << endl;
	cout << "���������� �� 3 ������� � ���������� ������ �������� ������� � 10 ���, �� 4 � � 20 ���, �� 5 � � 100 ���." << endl;
	cout << "���������� 1-2-3-4-5 �������� 125 �����, � 2-3-4-5-6 � 250 �����." << endl;
	cout << "���� ������ �������� �������������� (������ ����),�� ����� ����������� ������, ��������� ����," << endl;
	cout << "� ����� ����� ������� ���������� (���� � ����������� ����������� ��� 5 �������, �������� ���������� ���)." << endl;
	cout << "��, ���� � ��������� ������ ����� �������� 0 �����, �� ���� �� ���� ��� �������, � ������ ��������� � ������� ������." << endl;
	cout << "* ������� ������: ���� ����� \"��������\" �������, �.�. �� ���� �� ���� ������� ����� �����, � ����� ���� �������," << endl;
	cout << "(�� �� ������, ��� ������� ����� �� �������������!), �� ���������� ��������, � ���������� ������ 50 �����." << endl;
	cout << "* �����: ���� ����� �������� 100, 200, 300 � �.�. �����, �� �� �������� �� \"�����\" � ��� ����������� 175 �����." << endl;
	cout << "* ��������: ���� ����� �������� 150, 250, 350 � �.�. �����, �� �� �������� �� \"��������\" � � ���� ����������� 175 �����." << endl;
	cout << "======================================================= ������� =======================================================" << endl << endl;
}
bool StartGame(int& GameMode) {

	bool StartGameSelect = true;
	int StartGameSelect2 = 1;

	cout << "����� ���������� � ���������� \"���� � �����\"" << endl;

	do {
		if (StartGameSelect2 < 1 or StartGameSelect2 > 3) cout << "�������� ����!" << endl;
		cout << "[��� ������ ���� - ������� 1, ��� ��������� ������ - 2, ��� ������ �� ���� - 3]: "; cin >> StartGameSelect2;
	} while (StartGameSelect2 < 1 or StartGameSelect2 > 3);

	if (StartGameSelect2 == 3) StartGameSelect = false;

	if (StartGameSelect2 == 2) {

		ShowRules();

		do {
			if (StartGameSelect2 < 1 or StartGameSelect2 > 2) cout << "�������� ����!" << endl;
			cout << "������ ����? [1 - ��, 2 - �����]: "; cin >> StartGameSelect2;
		} while (StartGameSelect2 < 1 or StartGameSelect2 > 2);

		if (StartGameSelect2 == 2) StartGameSelect = false;
	}

	if (!StartGameSelect) return 0;

	if (StartGameSelect) {
		do {
			if (GameMode < 1 or GameMode > 2) cout << "�������� ����!" << endl;
			cout << "�������� ����� ���� [1 - ������ ����������, 2 - ��� ������]: "; cin >> GameMode;
		} while (GameMode < 1 or GameMode > 2);
	}

	return StartGameSelect;
}
void PressEnter() {
	char key = 0;
	cout << "[������� Enter ��� ������ ������]" << endl;
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

	cout << endl << "���������� �������: " << endl;

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
			cout << "[�����] ����� �������� 175 �����!" << endl << endl;
		}
		if (FirstPlayerTotalScore % 100 == 50 and FirstPlayerTotalScore != 0) {
			FirstPlayerTotalScore -= 175;
			cout << "[��������] ����� ������ 175 �����!" << endl << endl;
		}
		if (SecondPlayerTotalScore % 100 == 0 and SecondPlayerTotalScore != 0) {
			SecondPlayerTotalScore += 175;
			cout << "[�����] ��������� �������� 175 �����!" << endl << endl;
		}
		if (SecondPlayerTotalScore % 100 == 50 and SecondPlayerTotalScore != 0) {
			SecondPlayerTotalScore -= 175;
			cout << "[��������] ��������� ������ 175 �����!" << endl << endl;
		}
	}
	else {
		if (FirstPlayerTotalScore % 100 == 0 and FirstPlayerTotalScore != 0) {
			FirstPlayerTotalScore += 175;
			cout << "[�����] ������ ����� �������� 175 �����!" << endl << endl;
		}
		if (FirstPlayerTotalScore % 100 == 50 and FirstPlayerTotalScore != 0) {
			FirstPlayerTotalScore -= 175;
			cout << "[��������] ������ ����� ������ 175 �����!" << endl << endl;
		}
		if (SecondPlayerTotalScore % 100 == 0 and SecondPlayerTotalScore != 0) {
			SecondPlayerTotalScore += 175;
			cout << "[�����] ������ ����� �������� 175 �����!" << endl << endl;
		}
		if (SecondPlayerTotalScore % 100 == 50 and SecondPlayerTotalScore != 0) {
			SecondPlayerTotalScore -= 175;
			cout << "[��������] ������ ����� ������ 175 �����!" << endl << endl;
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
			cout << "[�����] ��������� ������� ������." << endl;
			cout << "[�����] ��������� �������� 50 �����." << endl;
			cout << "[�����] ����� ������ 50 �����." << endl << endl;
		}
		else if (Max == 2 and FirstPlayerTotalScore > SecondPlayerTotalScore) {
			FirstPlayerTotalScore += 50;
			SecondPlayerTotalScore -= 50;
			Max = 1;
			cout << "[�����] ����� ������� ���������." << endl;
			cout << "[�����] ����� �������� 50 �����." << endl;
			cout << "[�����] ��������� ������ 50 �����." << endl << endl;
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
			cout << "[�����] ������ ����� ������� �������." << endl;
			cout << "[�����] ������ ����� �������� 50 �����." << endl;
			cout << "[�����] ������ ����� ������ 50 �����." << endl << endl;
		}
		else if (Max == 2 and FirstPlayerTotalScore > SecondPlayerTotalScore) {
			FirstPlayerTotalScore += 50;
			SecondPlayerTotalScore -= 50;
			Max = 1;
			cout << "[�����] ������ ����� ������� �������." << endl;
			cout << "[�����] ������ ����� �������� 50 �����." << endl;
			cout << "[�����] ������ ����� ������ 50 �����." << endl << endl;
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