#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>

class Track
{
	// Класс трек
public:
	std::string name;
	std::string singer;
	std::tm dateCreate;
	double duration;
};

class Player
{
	// Класс плей-лист. Содержит список треков и медоты работы с ними
public:
	std::vector<Track> playList;

	int play()
	{	// Метод Play вводим с клавиатуры название трека, ищем его в плей-листе
		// Если находим, то воспроизводим.
		// Функция возвращает номер трека в плейлисте.
		bool found = false;
		int i = 0;
		std::string name;

		std::cout << "\nEnter name of track >";
		std::cin.clear();
		std::getline(std::cin, name);
		while (i < playList.size() && !found)
		{
			if (name == playList[i].name)
				found = true;
			else
				++i;
		}
		if (!found)
			 i = -1;
		return i;
	}
	
	std::string pause(int trackNumber)
	{	// Функция возвращает строку в которой сообщается, что
		// Трек с именем ... приостановлен
		return "Track: \"" + playList[trackNumber].name + "\" is suspended.";
	}

	int next()
	{	// Функция возвращает номер другого трека в плей-листе
		return rand() % playList.size();
	}

	std::string stop()
	{	// Функция возвращает сообщение, что плеер остановлен
		return "The player is stoped.";
	}

	void dispAllTrack()
	{	// Функция отображает содержимое плей-листа и ничего не возвращает.
		std::cout << '\n';
		for (int i = 0; i < playList.size(); ++i)
		{
			std::cout << playList[i].name << '\t' << playList[i].singer << '\t' << std::put_time(&playList[i].dateCreate, "%d.%m.%Y")<< '\t' << playList[i].duration << '\n';
		}
	}

	std::string add()
	{	// Функция считывает с клавиатуры параметры трека и сохраняет в плей-лист
		// Функция возвращает строку об успешном добавлении треков в плей-лист
		std::string name, singer;
		std::tm dateCreate = {};
		double duration;

		std::cout << "\nEnter name of track >";
		getline(std::cin, name);
		std::cout << "Enter name of musician >";
		getline(std::cin, singer);
		std::cout << "Enter date creation >";
		std::cin >> std::get_time(&dateCreate, "%d.%m.%Y");
		std::cout << "Enter duration of track >";
		std::cin >> duration;
		playList.push_back({name, singer, dateCreate, duration});
		return "Track added successfully.";
	}

	std::string generate()
	{	// Функция генерирует указанное с клавиатуры число треков и записывает их в плей-лист
		// Возвращает сообщение об успешном пополнении плей-листа
		std::string name, singer;
		std::tm dateCreate = {};
		double duration;
		int trackNumber;
		
		std::cout << "\nHow many tracks to generate? >";
		std::cin >> trackNumber;
		for(int i=0; i< trackNumber;++i)
		{
			dateCreate.tm_year = std::rand() % 99;
			dateCreate.tm_mday = std::rand() % 28 + 1;
			dateCreate.tm_mon = std::rand() % 11 + 1;
			name = "Name " + std::to_string(i);
			singer = "Singer " + std::to_string(i);
			duration = std::rand() % 600;
			playList.push_back({ name, singer, dateCreate, duration });
		}
		return "Traks was generated.";
	}
};

int main()
{
	char cmd = ' ';
	std::string message = "";
	Player* player = new Player();
	int trackNumber = -1;
	char status = '0';	// '0' - не используется, '1' - воспроизводится, '2' - пауза, '3' - стоп

	while(cmd != 'E')
	{	// Формируем меню
		std::cout << "A - Add track\nG - generate track list\nD - Display all track\nP - Play\nU - Pause\nN - Next record\nS - Stop play\nE - Exit\n" << message << '\n' << "Your choice >";
		cmd = _getch();
		if (cmd > 127)
			cmd = _getch();
		// Преобразование маленьких букв в большие
		if (cmd > 96)
			cmd -= 32;
		if (cmd == 'A')
		{	// Воод треков с клавиатуры
			message = player->add();
			std::cin.get();
		}
		else if (cmd == 'P')
		{	// Воспроизведение трека
			if (trackNumber < 0)
			{	// Если плеер не воспроизводит трек, то вызыаем процедуру поиска и воспроизведения трека
				trackNumber = player->play();
				message = "Plaing track: \"" + player->playList[trackNumber].name + "\"";
				// Устанавливаем статус: Воспроизведение
				status = '1';
			}
			else if (status == '2')
			{	// Если трек стоит на паузе, то мы продолжаем воиспроизведение
				message = "Plaing track: \"" + player->playList[trackNumber].name + "\"";
				status = '1';
			}
		}
		else if (cmd == 'U')
		{	// Если трек воспроизводится, то ставим на паузу, иначе ничего не делаем
			if (status == '1')
			{
				message = player->pause(trackNumber);
				status = '2';
			}
			else
				message = "";
		}
		else if (cmd == 'N')
		{	// Берём другой трек и воспроизводим
			trackNumber = player->next();
			message = "Plaing track: \"" + player->playList[trackNumber].name + "\"";
			status = '1';
		}
		else if (cmd == 'S')
		{	// Если плеер не остановлен - останавливаем.
			if (status != '0')
			{
				message = player->stop();
				trackNumber = -1;
				status = '0';
			}
		}
		else if (cmd == 'D')
		{	// Выводим на экран содержимое плей-листа
			player->dispAllTrack();
			// Ждём нажатия любой клавиши
			std::cout << "Press any key ...";
			while (!_kbhit())
			{
			}
			message = "";
		}
		else if (cmd == 'G')
		{	// Генерируем плей-лист
			message = player->generate();
			std::cin.get();
		}
		else
			message = std::string() + cmd + " - Undefined command";
	std::cout << "\x1B[2J\x1B[H";
	}
	delete player;
	player = nullptr;
	std::cout << "Thank you for use my program.\n";
}