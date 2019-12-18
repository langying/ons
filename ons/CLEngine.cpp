//
//  CLEngine.cpp
//  CLite
//
//  Created by 韩琼 on 2019/11/27.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include <dirent.h>

#include "CLLex.hpp"
#include "CLVar.hpp"
#include "CLEngine.hpp"

using namespace std;

void ons_dump(std::string& func, CLVar* args, void* obj) {
    CLOns* ons = (CLOns*)obj;
    ons->root->trace(">  ");
}

void ons_automode(CLVar* func, void* data) {
    ((CLEngine*)data)->win->automode = true;
}
void ons_automode_time(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->automode_time = val->getInt();
}
void ons_br(CLVar* func, void* data) {
    ((CLEngine*)data)->win->br();
}
void ons_bg(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->bg = val->getString();
}
void ons_bgm(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->bgm.file = val->getString();
}
void ons_bgmstop(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->bgm.status = val->getInt();
}
void ons_btnwait(CLVar* func, void* data) {
    std::cout << "btnwait>>>: ";
    int num = 0;
    ((CLEngine*)data)->ons->request(num);
    func->getParameter("0")->setInt(num);
}
void ons_btnwait2(CLVar* func, void* data) {
    ons_btnwait(func, data);
}
void ons_caption(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->caption = val->getString();
}
void ons_csp(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->csp(val->getInt());
}
void ons_csp2(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->csp2(val->getInt());
}
void ons_click(CLVar* func, void* data) {
    std::cout << "click>>>: ";
    int num = 0;
    ((CLEngine*)data)->ons->request(num);
}
void ons_dwave(CLVar* func, void* data) {
    CLOgg ogg;
    ogg.id   = func->getParameter("0")->getInt();
    ogg.file = func->getParameter("1")->getString();
    ((CLEngine*)data)->win->dwave(ogg);
}
void ons_dwavestop(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->dwavestop(val->getInt());
}
void ons_dwaveloop(CLVar* func, void* data) {
    CLOgg ogg;
    ogg.id   = func->getParameter("0")->getInt();
    ogg.file = func->getParameter("1")->getString();
    ((CLEngine*)data)->win->dwaveloop(ogg);
}
void ons_effect(CLVar* func, void* data) {
    CLEffect eff;
    eff.id       = func->getParameter("0")->getInt();
    eff.effect   = func->getParameter("1")->getInt();
    eff.duration = func->getParameter("2")->getInt();
    int argc = func->getParameter(ONS_ARGC)->getInt();
    if (argc > 3) {
        eff.image = func->getParameter("3")->getString();
    }
    ((CLEngine*)data)->win->effects[eff.id] = eff;
}
void ons_effectblank(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->effectblank = val->getInt();
}
void ons_erasetextwindow(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->erasetextwindow(val->getInt());
}
void ons_filelog(CLVar* func, void* data) {
    ((CLEngine*)data)->win->filelog = true;
}
void ons_game(CLVar* func, void* data) {
    ((CLEngine*)data)->ons->jump("*start");
}
void ons_getspmode(CLVar* func, void* data) {
    int spid = func->getParameter("1")->getInt();
    CLImg img = ((CLEngine*)data)->imgs[spid];
    func->getParameter("0")->setInt(img.hidden?0:1);
}
void ons_getspsize(CLVar* func, void* data) {
    int argc = func->getParameter(ONS_ARGC)->getInt();
    int spid = func->getParameter("0")->getInt();
    
    CLImg img = ((CLEngine*)data)->imgs[spid];
    func->getParameter("1")->setInt(img.w);
    func->getParameter("2")->setInt(img.h);
    if (argc > 3) {
        func->getParameter("3")->setInt(img.n); // !TODO: 看不懂
    }
}
void ons_globalon(CLVar* func, void* data) {
    ((CLEngine*)data)->win->globalon = true;
}
void ons_jumpb(CLVar* func, void* data) {
    ((CLEngine*)data)->win->jump('b');
}
void ons_jumpf(CLVar* func, void* data) {
    ((CLEngine*)data)->win->jump('f');
}
void ons_humanz(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->humanz = val->getInt();
}
void ons_kidokuskip(CLVar* func, void* data) {
    // !TODO: 初次阅读的判断，禁止跳过
    // ((CLEngine*)data)->win->kidoku;
}
void ons_labellog(CLVar* func, void* data) {
    ((CLEngine*)data)->win->labellog = true;
}
void ons_lookbackcolor(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->lookbackcolor = val->getInt();
}
void ons_lsp(CLVar* func, void* data) {
    int     id = func->getParameter("0")->getInt();
    string  ss = func->getParameter("1")->getString();
    int     px = func->getParameter("2")->getInt();
    int     py = func->getParameter("3")->getInt();
    int     aa = func->getParameter("4")->getInt();
    ((CLEngine*)data)->win->lsp(id, ss, px, py, aa);
}
void ons_lsp2(CLVar* func, void* data) {
    int     id = func->getParameter("0")->getInt();
    string  ss = func->getParameter("1")->getString();
    int     px = func->getParameter("2")->getInt();
    int     py = func->getParameter("3")->getInt();
    int     sx = func->getParameter("4")->getInt();
    int     sy = func->getParameter("5")->getInt();
    int     rr = func->getParameter("6")->getInt();
    int     aa = func->getParameter("7")->getInt();
    ((CLEngine*)data)->win->lsp2(id, ss, px, py, sx, sy, rr, aa);
}
void ons_msp(CLVar* func, void* data) {
    int id = func->getParameter("0")->getInt();
    int dx = func->getParameter("1")->getInt();
    int dy = func->getParameter("2")->getInt();
    int da = func->getParameter("3")->getInt();
    ((CLEngine*)data)->win->msp(id, dx, dy, da);
}
void ons_msp2(CLVar* func, void* data) {
    int id = func->getParameter("0")->getInt();
    int dx = func->getParameter("1")->getInt();
    int dy = func->getParameter("2")->getInt();
    int sx = func->getParameter("3")->getInt();
    int sy = func->getParameter("4")->getInt();
    int dr = func->getParameter("5")->getInt();
    int da = func->getParameter("5")->getInt();
    ((CLEngine*)data)->win->msp2(id, dx, dy, sx, sy, dr, da);
}
void ons_mpegplay(CLVar* func, void* data) {
    string mv = func->getParameter("0")->getString();
    int    id = func->getParameter("1")->getInt();
    ((CLEngine*)data)->win->mpegplay(id, mv);
}
void ons_menuselectcolor(CLVar* func, void* data) {
    CLCss& menu = ((CLEngine*)data)->win->menu;
    menu.color1 = func->getParameter("0")->getInt(); // 正常
    menu.color2 = func->getParameter("1")->getInt(); // 高光
    menu.color3 = func->getParameter("2")->getInt(); // 低光
}
void ons_menusetwindow(CLVar* func, void* data) {
    CLCss& menu = ((CLEngine*)data)->win->menu;
    menu.font.width     = func->getParameter("0")->getInt();
    menu.font.height    = func->getParameter("1")->getInt();
    menu.origin.x       = func->getParameter("2")->getInt() + menu.font.width;
    menu.origin.y       = func->getParameter("3")->getInt() + menu.font.height;
    menu.bold           = func->getParameter("4")->getInt() ? true : false;
    menu.shadow         = func->getParameter("5")->getInt() ? true : false;
    menu.bgColor        = func->getParameter("6")->getInt();
}
void ons_nsa(CLVar* func, void* data) {
    CLEngine* engine = (CLEngine*)data;
    vector<string> list;
    engine->getPathFiles(list, ".nsa", ".NSA");
    for (string& name : list) {
        engine->loadNsa(name);
    }
}
void ons_print(CLVar* func, void* data) {
    int argc = func->getParameter(ONS_ARGC)->getInt();
    if (argc < 2) {
        ((CLEngine*)data)->print(0, 0);
    } else {
        int anim = func->getParameter("0")->getInt();
        int time = func->getParameter("1")->getInt();
        ((CLEngine*)data)->print(anim, time);
    }
}
void ons_reset(CLVar* func, void* data) {
    ((CLEngine*)data)->reset();
}
void ons_rmenu(CLVar* func, void* data) {
    CLWin* win = ((CLEngine*)data)->win;
    int argc = func->getParameter(ONS_ARGC)->getInt();
    for (int i = 0; i < argc; i++) {
        CLVar* val = func->getParameter(to_string(i));
        win->rmenu.push_back(val->getString());
    }
}
void ons_rmode(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->rmode = val->getInt();
}
void ons_skip(CLVar* func, void* data) {
}
void ons_savename(CLVar* func, void* data) {
     CLWin* win = ((CLEngine*)data)->win;
     int argc = func->getParameter(ONS_ARGC)->getInt();
     for (int i = 0; i < argc; i++) {
         CLVar* val = func->getParameter(to_string(i));
         win->savename.push_back(val->getString());
     }
}
void ons_savenumber(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->savenumber = val->getInt();
}
void ons_selectcolor(CLVar* func, void* data) {
    CLCss& label = ((CLEngine*)data)->win->label;
    label.color1 = func->getParameter("0")->getInt();
    label.color2 = func->getParameter("1")->getInt();
}
void ons_setwindow(CLVar* func, void* data) {
    int a = func->getParameter("0")->getInt();
    int b = func->getParameter("1")->getInt();
    int c = func->getParameter("2")->getInt();
    int d = func->getParameter("3")->getInt();
    int e = func->getParameter("4")->getInt();
    int f = func->getParameter("5")->getInt();
    int g = func->getParameter("6")->getInt();
    int h = func->getParameter("7")->getInt();
    int i = func->getParameter("8")->getInt();
    int j = func->getParameter("9")->getInt();
    int k = func->getParameter("10")->getInt();
    int l = func->getParameter("11")->getInt();
    int m = func->getParameter("12")->getInt();
    int n = func->getParameter("13")->getInt();
    int o = func->getParameter("14")->getInt();
    int p = func->getParameter("15")->getInt();
    ((CLEngine*)data)->win->setwindow(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);
}
void ons_spbtn(CLVar* func, void* data) {
    int id = func->getParameter("0")->getInt();
    int vv = func->getParameter("1")->getInt();
    ((CLEngine*)data)->win->spbtn(id, vv);
}
void ons_systemcall(CLVar* func, void* data) {
    int argc = func->getParameter(ONS_ARGC)->getInt();
    cout << "systemcall";
    for (int i=0; i<argc; i++) {
        cout << " " << func->getParameter(to_string(i))->getString();
    }
    cout << endl;
//    CLEngine* engine = (CLEngine*)data;
//    string cmd = func->getParameter("0")->getString();
//    if (cmd == "end") {
//        engine->end();
//    } else if (cmd == "load") {
//        engine->load();
//    } else if (cmd == "save") {
//        engine->save();
//    } else if (cmd == "skip") {
//        engine->skip();
//    } else if (cmd == "reset") {
//        engine->reset();
//    } else if (cmd == "rmenu") {
//        engine->rmenu();
//    } else if (cmd == "automode") {
//        engine->automode();
//    } else if (cmd == "lookback") {
//        engine->lookback();
//    }
}
void ons_textoff(CLVar* func, void* data) {
    ((CLEngine*)data)->win->textoff();
}
void ons_textgosub(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->textgosub = val->getString();
}
void ons_transmode(CLVar* func, void* data) {
    CLVar* val = func->getParameter("0");
    ((CLEngine*)data)->win->transmode = val->getString();
}
void ons_versionstr(CLVar* func, void* data) {
    CLVar* v1 = func->getParameter("0");
    CLVar* v2 = func->getParameter("1");
    CLWin* win = ((CLEngine*)data)->win;
    win->versionstr[0] = v1->getString();
    win->versionstr[1] = v2->getString();
}
void ons_wait(CLVar* func, void* data) {
}

void ons_windowback(CLVar* func, void* data) {
    ((CLEngine*)data)->win->windowback = true;
}


CLEngine::~CLEngine() {
    if (ons) {
        delete ons;
    }
    if (win) {
        delete win;
    }
    for (auto it : nsas) {
        delete it.second;
    }
    nsas.clear();
}

CLEngine::CLEngine() {
    ons = NULL;
    win = NULL;
}
void CLEngine::reset() {
    ons->jump("*define");
    cout << "game reset to *define" << endl;
}
void CLEngine::load(const char* _path) {
    this->path = _path;
    // !TODO: 检查加密文件
    
    vector<string> list;
    getPathFiles(list, ".txt", ".TXT");
    for (string& name : list) {
        std::string pathfile = path + "/" + name;
        std::ifstream file(pathfile);
        std::istreambuf_iterator<char> begin(file);
        std::istreambuf_iterator<char> end;
        std::string text(begin, end);
        loadTxt(name, text);
    }
    
    ons = new CLOns(code);
    ons->addFunction("automode",        ons_automode,           this);
    ons->addFunction("automode_time",   ons_automode_time,      this);
    ons->addFunction("br",              ons_br,                 this);
    ons->addFunction("bg",              ons_bg,                 this);
    ons->addFunction("bgm",             ons_bgm,                this);
    ons->addFunction("bgmstop",         ons_bgmstop,            this);
    ons->addFunction("btnwait",         ons_btnwait,            this);
    ons->addFunction("btnwait2",        ons_btnwait2,           this);
    ons->addFunction("caption",         ons_caption,            this);
    ons->addFunction("csp",             ons_csp,                this);
    ons->addFunction("csp2",            ons_csp2,               this);
    ons->addFunction("click",           ons_click,              this);
    ons->addFunction("dwave",           ons_dwave,              this);
    ons->addFunction("dwavestop",       ons_dwavestop,          this);
    ons->addFunction("dwaveloop",       ons_dwaveloop,          this);
    ons->addFunction("effect",          ons_effect,             this);
    ons->addFunction("effectblank",     ons_effectblank,        this);
    ons->addFunction("erasetextwindow", ons_erasetextwindow,    this);
    ons->addFunction("filelog",         ons_filelog,            this);
    ons->addFunction("game",            ons_game,               this);
    ons->addFunction("getspmode",       ons_getspmode,          this);
    ons->addFunction("getspsize",       ons_getspsize,          this);
    ons->addFunction("globalon",        ons_globalon,           this);
    ons->addFunction("humanz",          ons_humanz,             this);
    ons->addFunction("jumpb",           ons_jumpb,              this);
    ons->addFunction("jumpf",           ons_jumpf,              this);
    ons->addFunction("kidokuskip",      ons_kidokuskip,         this);
    ons->addFunction("labellog",        ons_labellog,           this);
    ons->addFunction("lookbackcolor",   ons_lookbackcolor,      this);
    ons->addFunction("lsp",             ons_lsp,                this);
    ons->addFunction("lsp2",            ons_lsp2,               this);
    ons->addFunction("msp",             ons_msp,                this);
    ons->addFunction("msp2",            ons_msp2,               this);
    ons->addFunction("mpegplay",        ons_mpegplay,           this);
    ons->addFunction("menuselectcolor", ons_menuselectcolor,    this);
    ons->addFunction("menusetwindow",   ons_menusetwindow,      this);
    ons->addFunction("print",           ons_print,              this);
    ons->addFunction("reset",           ons_reset,              this);
    ons->addFunction("rmenu",           ons_rmenu,              this);
    ons->addFunction("rmode",           ons_rmode,              this);
    ons->addFunction("skip",            ons_skip,               this);
    ons->addFunction("savename",        ons_savename,           this);
    ons->addFunction("savenumber",      ons_savenumber,         this);
    ons->addFunction("selectcolor",     ons_selectcolor,        this);
    ons->addFunction("setwindow",       ons_setwindow,          this);
    ons->addFunction("spbtn",           ons_spbtn,              this);
    ons->addFunction("systemcall",      ons_systemcall,         this);
    ons->addFunction("textoff",         ons_textoff,            this);
    ons->addFunction("textgosub",       ons_textgosub,          this);
    ons->addFunction("transmode",       ons_transmode,          this);
    ons->addFunction("nsa",             ons_nsa,                this);
    ons->addFunction("versionstr",      ons_versionstr,         this);
    ons->addFunction("wait",            ons_wait,               this);
    ons->addFunction("windowback",      ons_windowback,         this);
    win = new CLWin(ons);
    reset();
}


void CLEngine::loadNsa(const string& name) {
    nsas[name] = new CLNsa(path + "/" + name);
}
void CLEngine::loadTxt(const string& name, const string& text) {
    CLTxt txt(name, text);
    if (code.length()){
        code += '\n';
    }
    code += text;
    txts.push_back(txt);
}

void CLEngine::execute() {
    ons->execute();
}

void CLEngine::response(int num) {
    ons->response(num);
}

void CLEngine::set(const std::string& key, const std::string& val) {
    data[key]=val;
}

void CLEngine::print(int anim, int time) {
    
}

void CLEngine::getPathFiles(vector<string>& list, const string& ext1, const string& ext2) {
    DIR* dir;
    struct dirent *ptr;
    if ((dir = opendir(path.c_str())) == NULL) {
        std::cout << "Open dir error:" << path;
        return;
    }
    
    while ((ptr = readdir(dir))) {
        std::string name = ptr->d_name;
        if (ptr->d_type != DT_REG || name == "." || name == "..") {
            continue;
        }
        if (endWith(name, ext1) || endWith(name, ext1)) {
            list.push_back(name);
        }
    }
    closedir(dir);
    sort(list.begin(), list.end());
}

