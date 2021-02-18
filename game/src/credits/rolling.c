#include "device/video.h"
// how many pixels each time
#define ROLL_SPEED 200

static int roll_tick = 200;

static char *credits_text[] = {
	" ",	
	" ",
	"\t\t\t\tNju EMUlator v3.3",
	"\t\t\t\tFall, 2020",
	"\t\t\t\tWe got online!",
	" ",
	" ",
	"\t\t\t\tHistory of NEMU:",
        " ",
	"\t\t\t\tNEMU v3.2 - Fall, 2019",
        " ",
	"\t\t\t\tNEMU v3.1 - Spring, 2019",
        " ",
	"\t\t\t\tNEMU v3.0 - Fall, 2018",
	" ",
	"\t\t\t\tNEMU v2.0 - Fall, 2017",
	" ",
	"\t\t\t\tNEMU v1.0 - Fall, 2014",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",

	" ",
	"\t\t\t\t\t= DEVELOPMENT TEAM =",
	" ",
	" ",
	"\t\t\t\t\tLeading Developers:",
	" ",
	"\t\t\t\t\t  LIANG WANG\t\t\tZIHAO YU",
	" ",
	" ",
	"\t\t\t\t\tCore Contributors:",
	"\t\t\t\t\t(in alphabetical order)",
	" ",
	"\t\t\t\t\t  ZIHAN CUI (16CS, NJU)",
	" ",
	"\t\t\t\t\t  ZHIHAN HE (15CS, NJU)",
	" ",
	"\t\t\t\t\t  LINTIAN SHI (16CS, NJU)",
	" ",
	"\t\t\t\t\t  YAXIN WANG (16CS, NJU)",
	" ",
	"\t\t\t\t\t  MINGYA ZHANG (PhD, NJU)",
	" ",
	"\t\t\t\t\t  YUQIAN ZHUANG (PhD, NJU)",
	" ",
	" For Submission: you can turn it off now",

	" ",
	"\t\t\t\t\tContributors & Bug Reporters:",
	"\t\t\t\t\t(by time)",
	" ",
	"\t\t\t\t\t  LIUJIA LI (18CS, NJU)",
	" ",
	"\t\t\t\t\t  TING XU (17CS, NJU)",
	" ",
	"\t\t\t\t\t  PEIWEN YU (16CS, NJU)",
	" ",
	"\t\t\t\t\t  TIAN XIE (16CS, NJU)",
	" ",
	"\t\t\t\t\t  MENGYAO ZHANG (16CS, NJU)",
	" ",
	"\t\t\t\t\t  XIANG XU (16CS, NJU)",
	" ",
	"\t\t\t\t\t  FENGBIN HE (16CS, NJU)",
	" ",
	"\t\t\t\t\t  MINGCHAO ZHANG (16CS, NJU)",
	" ",
	"\t\t\t\t\t  HANGFAN ZHANG (16CS, NJU)",
	" ",
	"\t\t\t\t\t  YUHU YAN (16CS, NJU)",
	" ",
	" For Submission: you can turn it off now",

	" ",
	" ",
	"\t\t\t\t\t= TEACHING TEAM =",
	" ",
	" ",
	"\t\t\t\t\tCourse Teachers:",
	" ",
	"\t\t\t\t\t  CHUNFENG YUAN",
	" ",
	"\t\t\t\t\t  TONG LU",
	" ",
	"\t\t\t\t\t  JIE TANG",
	" ",
	" ",
	" ",
	"\t\t\t\t\tLab Teacher:",
	" ",
	"\t\t\t\t\t  FENG SU",
	" ",
	" ",
	" ",
	"\t\t\t\t\tPA (NEMU) Teacher:",
	" ",
	"\t\t\t\t\t  LIANG WANG",
	" For Submission: you can turn it off now",

	" ",
	"\t\t\t\t\tTeaching Assistants:",
	" ",
	"\t\t\t\t\t  YUQIAN ZHUANG (Since 2019)",
	"\t\t\t\t\t  YANDONG ZHENG (Spring, 2020)",
	"\t\t\t\t\t  TIANHENG WU (Spring, 2018 - 2019)",
	"\t\t\t\t\t  ZHIHAN HE (Fall, 2017)",
	"\t\t\t\t\t  MINGJIE SHEN (Fall, 2017)",
	"\t\t\t\t\t  RONGCHUN YAO (Fall, 2017)",
	"\t\t\t\t\t  HAORAN DING (Fall, 2016)",
	"\t\t\t\t\t  WENPU CAI (Fall, 2016)",
	"\t\t\t\t\t  PENGGUANG CHEN (Fall, 2016)",
	"\t\t\t\t\t  ZIHAO YU (Fall, 2015, 2014)",
	"\t\t\t\t\t  LIANG WANG (Fall, 2015, 2014)",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ", 
	" For Submission: you can turn it off now",

	" ",
	" ",
	" ",
	" ",
	" ",
	" Nanjing University",
	" ",
	" ",
	"   #     #  #######  #     #  #     #",
	"   ##    #  #        ##   ##  #     #",
	"   # #   #  #        # # # #  #     #",
	"   #  #  #  #####    #  #  #  #     #",
	"   #   # #  #        #     #  #     #",
	"   #    ##  #        #     #  #     #",
	"   #     #  #######  #     #   ##### ",
	" ",
	"                                 v3.3",
	" ",
	" ======= END =======",
	" ",
	" ",
	" ",
	" ",
	" ",
	" For Submission: you can turn it off now",
	" ", // need this?
};

#define NR_TXT_LINES (sizeof(credits_text) / sizeof(credits_text[0]))

void roll_text()
{
	int lines = roll_tick >> 3;
	if (lines > NR_TXT_LINES)
		return;
	int partial = lines == NR_TXT_LINES ? 0 : roll_tick - (lines << 3);
	lines = lines <= NR_TXT_LINES ? lines : NR_TXT_LINES;
	int i = lines - 1;
	int x = SCR_HEIGHT - partial;
	prepare_buffer();
	for (; i >= 0; i--)
	{
		x -= 8;
		if (x < 0)
			break;
		draw_string(credits_text[i], x, 0, 15);
	}
	display_buffer();
	roll_tick += ROLL_SPEED;
}
