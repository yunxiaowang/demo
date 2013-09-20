#include <QApplication>
#include <QNetworkProxy>
#include "Container.h"
#include <stdlib.h>

//#define DEBUG

#ifdef DEBUG
#include <signal.h>
#endif

Container * pflashContainer;

void PrintChoice()
{
    printf("################### Make a choice! #####################\n");
    printf("1. Play a vidio from www.youku.com, make sure you can access the Internet!\n");
    printf("2. Play a flash game from www.5703.com, make sure you can access the Internet!\n");
    printf("3. Play a local flash\n");
    printf("########################################################\n");
}

#ifdef DEBUG
void dump(int signo)
{
        char buf[1024];
        char cmd[1024];
        FILE *fh;

        snprintf(buf, sizeof(buf), "/proc/%d/cmdline", getpid());
        if(!(fh = fopen(buf, "r")))
                exit(0);
        if(!fgets(buf, sizeof(buf), fh))
                exit(0);
        fclose(fh);
        if(buf[strlen(buf) - 1] == '/n')
                buf[strlen(buf) - 1] = '/0';
        snprintf(cmd, sizeof(cmd), "gdb %s %d", buf, getpid());
        system(cmd);

        exit(0);
}

#endif

int main(int argc, char * argv[])
{
    QApplication a(argc, argv);

#ifdef DEBUG
    signal(SIGSEGV, &dump);
#endif

    char choice = '\0';

    pflashContainer = new Container();

    int Flashid = 0;
    pflashContainer->CreateFlashInstance(&Flashid);
    pflashContainer->SetSize(800,800);
    std::map<std::string,std::string> * pPropertyMap = new std::map<std::string,std::string>();

    while(true)
    {
        PrintChoice();
        scanf("%c",&choice);

        switch(choice)
        {
        case '1':
            pPropertyMap->insert(std::make_pair("movie","http://static.youku.com/v1.0.0294/v/swf/loader.swf"));
            pPropertyMap->insert(std::make_pair("allowFullScreen","true"));
            pPropertyMap->insert(std::make_pair("flashvars","VideoIDS=XMzg3Nzg3ODQ0&amp;ShowId=254900&amp;category=84&amp;Cp=unknow&amp;Light=on&amp;THX=on&amp;unCookie=0&amp;frame=0&amp;pvid=1355967892190d5z&amp;Tid=0&amp;isAutoPlay=true&amp;Version=/v1.0.0826&amp;show_ce=1&amp;winType=interior&amp;embedid=Ajk2OTQ2OTYxAmppbHVwaWFuLnlvdWt1LmNvbQIvaW5kZXgvemlyYW4vX3BhZ2U1OTUzOF82Lmh0bWw="));
            pPropertyMap->insert(std::make_pair("quality","high"));
            pPropertyMap->insert(std::make_pair("pluginspage","http://www.macromedia.com/go/getflashplayer"));
            pPropertyMap->insert(std::make_pair("type","application/x-shockwave-flash"));
            pPropertyMap->insert(std::make_pair("width","200"));
            pPropertyMap->insert(std::make_pair("height","200"));
            break;
        case '2':
            pPropertyMap->insert(std::make_pair("movie","http://imgc.abab.com/small_flash_channel/intellect/empire_3303(1).swf"));
            pPropertyMap->insert(std::make_pair("allowFullScreen","true"));
            pPropertyMap->insert(std::make_pair("quality","high"));
            pPropertyMap->insert(std::make_pair("pluginspage","http://www.macromedia.com/go/getflashplayer"));
            pPropertyMap->insert(std::make_pair("type","application/x-shockwave-flash"));
            pPropertyMap->insert(std::make_pair("width","200"));
            pPropertyMap->insert(std::make_pair("height","200"));
            break;
        case '3':
            pPropertyMap->insert(std::make_pair("src","swf/firsttime.swf"));
            pPropertyMap->insert(std::make_pair("allowFullScreen","true"));
            pPropertyMap->insert(std::make_pair("quality","high"));
            pPropertyMap->insert(std::make_pair("pluginspage","http://www.macromedia.com/go/getflashplayer"));
            pPropertyMap->insert(std::make_pair("type","application/x-shockwave-flash"));
            pPropertyMap->insert(std::make_pair("width","200"));
            pPropertyMap->insert(std::make_pair("height","200"));
            break;
        default:
            break;
        }

        if('1' == choice || '2' == choice || '3' == choice)
        {
            break;
        }
    }
    pflashContainer->Property_Load(pPropertyMap,Flashid);

    int * pPos = new int[4];
    pPos[0] = 50;pPos[1] = 50;pPos[2] = 700;pPos[3] = 700;
    int * pClip = new int[4];
    pClip[0] = 50;pClip[1] = 50;pClip[2] = 700;pClip[3] = 700;
    pflashContainer->SetObjectRects(pPos,pClip,Flashid);

    pflashContainer->Move(50,100);

    a.exec();

    return 0;
}
