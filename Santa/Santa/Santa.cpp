#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include <random>
#include <boost/process.hpp>
#include <fstream>
#include <vector>

std::ofstream fout("log.txt");

int deers_num = 0;
int elves_num = 0;

bool elf_returned = false;
bool presents_delivered = false;

int all_threads_started = 0;

boost::mutex deers_mutex[9];
boost::mutex elves_mutex[9];
boost::condition_variable deers_cv;
boost::condition_variable elves_cv;

std::string AnimatedGrass(int i)
{
	std::string grass;
	switch (i)
	{
	case 12: grass = " \\\\\\\\||||////"; break;
	case 11: grass = " \\\\\\||||////"; break;
	case 10: grass = " \\\\\\||||///"; break;
	case 9: grass = " \\\\\\|||///"; break;
	case 8: grass = " \\\\|||///"; break;
	case 7: grass = " \\\\|||//"; break;
	case 6: grass = " \\\\||//"; break;
	case 5: grass = " \\||//";  break;
	case 4: grass = " \\||/"; break;
	case 3: grass = " \\|/";  break;
	case 2: grass = " |/";  break;
	case 1: grass = " /"; break;
	case 0: grass = " ";  break;
	}
	return grass;
}

std::string AnimatedWork(int i)
{
	std::string work;
	switch (i)
	{
	case 12: work = "||||||||||||"; break;
	case 11: work = "|||||||||||"; break;
	case 10: work = "||||||||||"; break;
	case 9: work = "|||||||||"; break;
	case 8: work = "||||||||"; break;
	case 7: work = "|||||||"; break;
	case 6: work = "||||||"; break;
	case 5: work = "|||||";  break;
	case 4: work = "||||"; break;
	case 3: work = "|||";  break;
	case 2: work = "||";  break;
	case 1: work = "|"; break;
	case 0: work = " ";  break;
	}
	return work;
}

enum SantaEvents
{
	SantaSleeps = 0x01,
	SantaDeliverPresents = 0x02,
	SantaHoldsMeeting = 0x04,
	SantaFallAsleep = 0x08,
};

struct Interface
{
	int deers[9] = { 0 };
	bool deers_grazing[9] = { false };
	int elves[9] = { 0 };
	bool elves_working[9] = { false };
	int Santa_event = SantaEvents::SantaSleeps;
} interface;

void DrawingUI()
{
	system("cls");
	for (int i = 0; i < 9; ++i)
	{
		if (interface.deers_grazing[i])
		{
			std::cout << "'Deer " << i + 1 << "' grazing\n"
				<< AnimatedGrass(interface.deers[i]) << "\n\n";
			fout << "'Deer " << i + 1 << "' grazing\n"
				<< AnimatedGrass(interface.deers[i]) << "\n\n";
		}
		else
		{
			std::cout << "'Deer " << i + 1 << "' waiting for santa\n\n";
			fout << "'Deer " << i + 1 << "' waiting for santa\n\n";
		}
	}
	for (int i = 0; i < 9; ++i)
	{
		if (interface.elves_working[i])
		{
			std::cout << "'Elf " << i + 1 << "' work progress is:\n"
				<< AnimatedWork(interface.elves[i]) << "\n\n";
			fout << "'Elf " << i + 1 << "' work progress is:\n"
				<< AnimatedWork(interface.elves[i]) << "\n\n";
		}
		else
		{
			std::cout << "'Elf " << i + 1 << "' waiting for santa\n\n";
			fout << "'Elf " << i + 1 << "' waiting for santa\n\n";
		}
	}
	switch (interface.Santa_event)
	{
	case SantaEvents::SantaFallAsleep:
		std::cout << "Santa falls asleep.\n\n";
		fout << "Santa falls asleep.\n\n";
		interface.Santa_event = SantaEvents::SantaSleeps;
		break;
	case SantaEvents::SantaDeliverPresents:
		std::cout << "Santa is waking up! Time to deliver presents.\n\n";
		fout << "Santa is waking up! Time to deliver presents.\n\n";
		break;
	case SantaEvents::SantaHoldsMeeting:
		std::cout << "Santa is waking up! Time for a meeting.\n\n";
		fout << "Santa is waking up! Time for a meeting.\n\n";
		break;
	default:
		break;
	}
	fout << "Count of free deers: " << deers_num << '\n';
	fout << "Count of free elves: " << elves_num << '\n';
	fout << "\n=============================================================================\n\n\n";
}

void elf(int param)
{
	boost::unique_lock<boost::mutex> lock(elves_mutex[param]);
	elves_cv.wait(lock, []() { return elf_returned; });
	++all_threads_started;
	while (true)
	{
		interface.elves_working[param] = true;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dis(1, 12);
		int n = dis(gen);
		int i = 0;
		while (i <= n)
		{
			interface.elves[param] = i;
			boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
			++i;
		}
		++elves_num;
		interface.elves_working[param] = false;
		elves_cv.wait(lock, []() {
			if (elf_returned)
			{
				elf_returned = 0;
				return true;
			}
			return false;
			});
	}
}

void deer(int param)
{
	boost::unique_lock<boost::mutex> lock(deers_mutex[param]);
	deers_cv.wait(lock, []() {return presents_delivered; });
	++all_threads_started;
	while (true)
	{
		interface.deers_grazing[param] = true;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dis(1, 12);
		int i = dis(gen);
		while (i != -1)
		{
			interface.deers[param] = i;
			boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
			i--;
		}
		++deers_num;
		interface.deers_grazing[param] = false;
		deers_cv.wait(lock, []() {return presents_delivered; });
		++deers_num;
	}
}

void santa()
{
	while (true)
	{
		if (deers_num == 9 || elves_num >= 3)
		{
			if (deers_num == 9)
			{
				interface.Santa_event = SantaEvents::SantaDeliverPresents;
				boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
				deers_num = 0;
				presents_delivered = true;
				boost::chrono::milliseconds(100);
				deers_cv.notify_all();
				while (deers_num != 9);
				deers_num -= 9;
				presents_delivered = false;
				interface.Santa_event = SantaEvents::SantaFallAsleep;
				continue;
			}
			else if (elves_num >= 3)
			{
				for (int i = 0; i < 3; ++i)
				{
					interface.Santa_event = SantaEvents::SantaHoldsMeeting;
					boost::this_thread::sleep_for(boost::chrono::milliseconds(300));
					elf_returned = true;
					elves_cv.notify_one();
				}
				elves_num -= 3;
				interface.Santa_event = SantaEvents::SantaFallAsleep;
			}
		}
	}
}

int created_creature = 0;
int loading_loop = 0;
bool santa_started_work = false;

std::string NextLoadingLoop()
{
	std::string loop;
	switch (loading_loop)
	{
	case 7: loop = "           .LPQBQqv.          \n        :KBBBBQBBBBBQK:       \n      dBBBBUi.   .rXBQBBb     \n    UBBB1    r.       uBBB1   \n   QBBP     .rjgB       XBBQ  \n  BBQi  7  ..:.7B        iBBB \n rBBB  q7      bB:        7QB:\n BBZIi:B  .  . gQB         QBD\nPQBi RB   .r  .BBB         rBg\nBQB: .B: ..RXLQBQQQBQ      :BR\nbBB7   7J7KQBQBBRgQBs      rBM\n BQB       KQu:BBQMQRSi:v: gQD\n rBBL           QBBMQBBBBBBBB:\n  BBB7           7BBMMgRgQBBQ \n   QBBd            qBgggMQBQ  \n    uBBB5.          UBMBBBJ   \n      bBBBBq7:....rsgBBBK     \n        .vEBBBQBBBBBP7        \n"; break;
	case 6: loop = "           .vqQQQPv.          \n        :KBBBBBBBBBBBX:       \n      bBBQBXr.. ..7KBBBQd     \n    1BBBs             1BBB1   \n   QBB1                 XQBQ  \n  BBQ:  :qD:             .BBQ \n 7BBr rR27IBvi            IBB:\n BBB     .:rBBQP 7BD   .QBQMQZ\nEMB7    .   iQQBBBBB5IBBBMggMg\nB5B: :.       QBQQRQBBBBMMDgDg\nbRBr  B   :.iLSBBBBBBBBBBBBggg\n BBB  .B    B5gBb .irri:rqUQQD\n rBBB7rQB  Ev:BJ          :BB:\n  BBBr .PXui             iBBQ \n   QBBu                 PBBQ  \n    uBBB1            .5BBBj   \n      bBBBBq7:...:7qBBBBP     \n        .vZBBBBBBBBBZL        \n"; break;
	case 5: loop = "           .vKQQQqv.          \n        :KBBQBBBBBBBgU.       \n      dBBBBK7.. ..:2BBBBq     \n    UBBBu          EQRMBBBJ   \n   BBB5            BBgggRQBQ  \n  BBBi             BRggQQBQBB \n rBBr         iBb:BBMgBQBsvBB:\n BBQ          rBQBBQQBB:   BQg\nbBBr   vU:UBBBBQQgBBBK     7Bg\nBQB:  BQZXBqBQBBQBQs       iBR\nPBBi IPi 1  r  QbQR        vBg\n BBQ .v :. .  :B.BB        BBD\n rBBr        r .P:i       sBB:\n  BBBr  .r:     B        7BBQ \n   BBBb   :rI2uUd       bBBQ  \n    UBBBJ   7PX.     .IBBBu   \n      bBBBBQS.   .7KBBBQb     \n         vPMBBBBBBBBEL        \n";  break;
	case 4: loop = "           .7SggD57.          \n        :XQBRMDgDRQBBS:       \n      bBBBBBQggDgMBEBBBBb     \n    1BBBu  BBQggDQQ  .IBBB1   \n   BBBX    .PBRDgBq     bBBQ  \n  BBBi       1BggBd      7BBQ \n rBQr        rBDMBg       YQB:\n BBQ       SBBgggBr        BBD\nbBBr        iBQgQB7        vBM\nBQQ:       .PBBBBMBB2      iBM\nPBBr       BBBq iZIgg      vBg\n BBQ     .ZBj i  P:r1:     BBD\n rBB7   LBP u .  r   gr   sBB:\n  BBBi  .EL ::      iBi  7BBQ \n   QBQS  iLr    :vSsRK  PBBQ  \n    1BBBL       .   KirBBBu   \n      bBBBBur..  .rBBQBBb     \n        .LDBBBBBBBBBq7        \n"; break;
	case 3: loop = "           .vKQQRKv.          \n        .IQBQBQBBBBBBS:       \n      qQBBR1r:. .:vPBQBBd     \n    uBQBRBU          .5BQBu   \n   QBQRDggBq            bBBQ  \n  QBBQgRgRMBB7           7BBB \n 7BQBBBBBQBRBBB           YBB:\n BBg :7:i5MQgQBB:uRK       BBg\nPBBr      JBBgRQBBBQq7J7   7Bg\nBQQ:      BBBQQBQvSR.. iB. :BM\nPBBr         BBB.  i.   BQ iBg\n BQQ         BBg .  .  B:i2EQg\n 7BB7        :Bb      vX  BBB:\n  BBBi        B7....  7  rBBB \n   BBBX       BgJr.     PBBQ  \n    uBQBu       .7    UBBB1   \n      bBBQB5r.   .iuBQBBP     \n        .YDBQBBBBBBBZL        \n";  break;
	case 2: loop = "           .vqQQQqv.          \n        :XBBBBBBBBBBBX:       \n      dBBBBPv:...:vPBBBBd     \n    1BBBI.            1BBB1   \n   QBQP                 1BBQ  \n  BBBi             r1XP. 7BBQ \n rBQ:          JB:vd  BBr7BQB:\n BBBUqr:irri. EBD5Q    B.  BBM\nPQQgBBBBBBBBBQBQXvi.:   B  rBg\nBQggDMRBBBBBRBQBQ       .: iB2\nPQRDgMBBB25QBBBBBQi   .    7BD\n BBRQBB.   DB7 PBBQr::  .  BBM\n rBBI            :vB572Qr 7BB:\n  BBB.             :DK:  iBBQ \n   QBBX                 2QBQ  \n    1BBB1             JBBBu   \n      PBBBBIr.. ..r5BBBBP     \n        .YDBQBBBBBQBDL.       \n";  break;
	case 1: loop = "           .vqQQQq7.          \n        :XBBBBQBQBBBQ5:       \n      dBBBBPv:   .qBBBBBd     \n    UBBB5.     :KP7   uBBBU   \n   QBBb       ZUu227:   bBBB  \n  QBB7        B     :r.  rBBB \n rBQY       i:q. r        rBB:\n BBB        BB.B:  . .: L  QBD\nPQBv        MBdR  r  1 rdU iBM\nBQBi       jQQQBBBBPBXERQ  :BM\nPBB7     PBBBgRQBBBBu:uv   rBg\n BBB   iBBQQQBBBr          QBg\n rBB7jBQBgMBB:PBi         rBB:\n  BBQBQQggMB             iBBB \n   QBBQggDQB            XBBQ  \n    JBBBMRBd          uBBB1   \n      KQBBBu:.. ..rSBBBQP     \n         75QBBBBQBBBZL        \n"; break;
	case 0: loop = "           .vPgQgdv.          \n        :5QBBBBBBBBBBX:       \n      dBBBBBr.  ..72BBBBd     \n    UBBBriK   .       sBBB1   \n   QBBq  qRYSv:    rYi  XQBB  \n  QBBr  rBi      :. YZ.  iBBB \n 7BBY   rD   r  . u dBv   7BB:\n BQB     :1riP  i uBE.     QQg\nbBBv      gg5Ei PBBB       rBg\nBQBi      IBBMBBBBq.       :QR\nPBBv        vBQgQBi        rBM\n BBB        rBgggBBS       QBg\n rBBY       gBgDBr        7BB:\n  BBB7      EBDgQU       iBBQ \n   QBBP     PQgDRBP.    KBBQ  \n    uBBBI   BQDgDQBB  1BBBu   \n      PBBQBPBggZggQBBBBQP     \n         LdQZgZDZDEEKv        \n";  break;
	}
	return loop;
}

void LoadingLoop()
{
	while (!santa_started_work)
	{
		system("cls");
		std::cout << "Waiting for the creation of all creatures\n\n";
		std::cout << NextLoadingLoop();
		if (!created_creature)
		{
			std::cout << "\n\nLoading...\n";
		}
		else
		{
			std::cout << "\n\nHello 'deer " << created_creature << "' and 'elf " << created_creature << "'! \n";
		}
		++loading_loop;
		loading_loop %= 8;
		boost::this_thread::sleep_for(boost::chrono::milliseconds(300));
	}
}

int main()
{
	std::vector <boost::thread*> elves;
	elves.reserve(9);
	std::vector <boost::thread*> deers;
	deers.reserve(9);
	boost::thread santa_th(santa);
	boost::thread loading_loop(LoadingLoop);

	for (int i = 0; i < 9; ++i)
	{
		{
			boost::lock_guard<boost::mutex> lock1(deers_mutex[i]);
			boost::lock_guard<boost::mutex> lock2(elves_mutex[i]);
		}
		boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
		elves.push_back(new boost::thread(elf, i));
		deers.push_back(new boost::thread(deer, i));
		created_creature = i + 1;
	}

	presents_delivered = true;
	elf_returned = true;

	elves_cv.notify_all();
	deers_cv.notify_all();

	while (all_threads_started != 18);

	elf_returned = false;
	presents_delivered = false;

	santa_started_work = true;
	while (true)
	{
		DrawingUI();
		boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
	}

	loading_loop.join();
	for (int i = 0; i < 9; ++i)
	{
		elves[i]->join();
		deers[i]->join();
	}
	santa_th.join();
}