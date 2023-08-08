#pragma once

class CBanSys
{
public:
  
    void Banaccount(char* name, char* reason, char* duration);
    void UnBanaccount(char* name);
    void LockAccount(char* name, char* reason);

    

};

