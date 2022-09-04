#include <windows.h>

int main()
{
    auto dbPasswd = "db161602241ec0dcc682d0ff6d481db010c686";

//------------------------------------------------------------

    auto symbolName = "";
    auto hmod = (HMODULE)0;

    WCHAR cwd[MAX_PATH] = { 0 };
    auto len = GetModuleFileNameW(NULL, cwd, MAX_PATH);
    while (len >= 0)
    {
        auto ch = cwd[len];
        cwd[len--] = 0;
        if ('\\' == ch)
        {
            break;
        }
    }
    SetCurrentDirectoryW(cwd);

    auto hmodKrnl32 = GetModuleHandleA("kernel32.dll");

    typedef void (*SIMPLE_CONSTRUCTOR)(LPVOID);

    auto hmodQt5Core = LoadLibraryA("Qt5Core.dll");
    auto hmodQt5Sql = LoadLibraryA("Qt5Sql.dll");
    auto hmodQSqlite = LoadLibraryA("QSqlite.dll");


    hmod = hmodQt5Core;
    symbolName = "?fromAscii_helper@QString@@CAPAU?$QTypedArrayData@G@@PBDH@Z";
    typedef LPVOID(*QSTRING_FROM_ASCII_HELPER)(LPCSTR, INT);
    auto fnQStringFromAsciiHelper = (QSTRING_FROM_ASCII_HELPER)GetProcAddress(hmod, symbolName);

    hmod = hmodQt5Sql;
    symbolName = "?defaultConnection@QSqlDatabase@@2PBDB";
    auto pQSqlDatabase_defaultConn = (LPCSTR*)GetProcAddress(hmod, symbolName);

    symbolName = "?addDatabase@QSqlDatabase@@SA?AV1@ABVQString@@0@Z";
    typedef LPVOID(*QSQLDATABASE_ADD_DATABASE_1)(LPVOID, LPVOID, LPVOID);
    auto fnQSqlDatabase_addDatabase1 = (QSQLDATABASE_ADD_DATABASE_1)GetProcAddress(hmod, symbolName);

    symbolName = "?setDatabaseName@QSqlDatabase@@QAEXABVQString@@@Z";
    auto fnQSqlDatabase_setDatabaseName = (LPVOID)GetProcAddress(hmod, symbolName);
    symbolName = "?setPassword@QSqlDatabase@@QAEXABVQString@@@Z";
    auto fnQSqlDatabase_setPassword = (LPVOID)GetProcAddress(hmod, symbolName);
    symbolName = "?setConnectOptions@QSqlDatabase@@QAEXABVQString@@@Z";
    auto fnQSqlDatabase_setConnectionOptions = (LPVOID)GetProcAddress(hmod, symbolName);
    symbolName = "?open@QSqlDatabase@@QAE_NXZ";
    auto fnQSqlDatabase_open = (LPVOID)GetProcAddress(hmod, symbolName);

//------------------------------------------------------------

    auto p1 = (LPBYTE)hmodQt5Core + 0x3f0b0;
    p1 = (LPBYTE)hmodQt5Core + 0xfae7c;
    

    len = lstrlenA((*pQSqlDatabase_defaultConn));
    auto qstrDbConn = fnQStringFromAsciiHelper((*pQSqlDatabase_defaultConn), len);
    auto qstrSqlDriver = fnQStringFromAsciiHelper("SQLITECIPHER", 12);
    
    DWORD err = 0;
    LPVOID qDatabase = 0;
    auto pQDatabase = fnQSqlDatabase_addDatabase1(&qDatabase, &qstrSqlDriver, &qstrDbConn);

    auto dbName = "tss.db";
    len = lstrlenA(dbName);
    auto qstrDbName = fnQStringFromAsciiHelper(dbName, len);
    __asm {
        lea eax, qstrDbName
        push eax
        mov ecx, pQDatabase
        mov eax, fnQSqlDatabase_setDatabaseName
        call eax
        mov err, eax
    }

    len = lstrlenA(dbPasswd);
    auto qstrPasswd = fnQStringFromAsciiHelper(dbPasswd, len);
    __asm {
        lea eax, qstrPasswd
        push eax
        mov ecx, pQDatabase
        mov eax, fnQSqlDatabase_setPassword
        call eax
        mov err, eax
    }

    auto connOptEmptyPassword = "QSQLITE_UPDATE_KEY=";
    len = lstrlenA(connOptEmptyPassword);
    auto qstrConnOptEmptyPassword = fnQStringFromAsciiHelper(connOptEmptyPassword, len);
    __asm {
        lea eax, qstrConnOptEmptyPassword
        push eax
        mov ecx, pQDatabase
        mov eax, fnQSqlDatabase_setConnectionOptions
        call eax
        mov err, eax
    }

    _asm {        
        mov ecx, pQDatabase
        mov eax, fnQSqlDatabase_open
        call eax
        mov err, eax
    }

    return 0;
}
