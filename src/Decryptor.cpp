#include <windows.h>

__interface IQSqlDatabase
{
    void Dummy();
};

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

    auto hmodQt5Core = LoadLibraryA("Qt5Core.dll");
    auto hmodQt5Sql = LoadLibraryA("Qt5Sql.dll");

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

    typedef LPVOID(__thiscall* QSQLDATABASE_SINGLE_PARAM_FUNC)(IQSqlDatabase*, LPVOID);

    symbolName = "?setDatabaseName@QSqlDatabase@@QAEXABVQString@@@Z";
    auto fnQSqlDatabase_setDatabaseName = (QSQLDATABASE_SINGLE_PARAM_FUNC)GetProcAddress(hmod, symbolName);
    symbolName = "?setPassword@QSqlDatabase@@QAEXABVQString@@@Z";
    auto fnQSqlDatabase_setPassword = (QSQLDATABASE_SINGLE_PARAM_FUNC)GetProcAddress(hmod, symbolName);
    symbolName = "?setConnectOptions@QSqlDatabase@@QAEXABVQString@@@Z";
    auto fnQSqlDatabase_setConnectionOptions = (QSQLDATABASE_SINGLE_PARAM_FUNC)GetProcAddress(hmod, symbolName);
    symbolName = "?open@QSqlDatabase@@QAE_NXZ";
    auto fnQSqlDatabase_open = (LPVOID)GetProcAddress(hmod, symbolName);

//------------------------------------------------------------

    len = lstrlenA((*pQSqlDatabase_defaultConn));
    auto qstrDbConn = fnQStringFromAsciiHelper((*pQSqlDatabase_defaultConn), len);
    auto qstrSqlDriver = fnQStringFromAsciiHelper("SQLITECIPHER", 12);
    
    DWORD err = 0;
    LPVOID qDatabase = 0;
    auto pQDatabase = (IQSqlDatabase*)fnQSqlDatabase_addDatabase1(&qDatabase, &qstrSqlDriver, &qstrDbConn);

    auto dbName = "tss.db";
    len = lstrlenA(dbName);
    auto qstrDbName = fnQStringFromAsciiHelper(dbName, len);
    fnQSqlDatabase_setDatabaseName(pQDatabase, &qstrDbName);

    len = lstrlenA(dbPasswd);
    auto qstrPasswd = fnQStringFromAsciiHelper(dbPasswd, len);
    fnQSqlDatabase_setPassword(pQDatabase, &qstrPasswd);

    auto connOptEmptyPassword = "QSQLITE_UPDATE_KEY=";
    len = lstrlenA(connOptEmptyPassword);
    auto qstrConnOptEmptyPassword = fnQStringFromAsciiHelper(connOptEmptyPassword, len);
    fnQSqlDatabase_setConnectionOptions(pQDatabase, &qstrConnOptEmptyPassword);

    _asm {        
        mov ecx, pQDatabase
        mov eax, fnQSqlDatabase_open
        call eax
        mov err, eax
    }

    return 0;
}
