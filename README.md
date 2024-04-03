# VekProcessInjector

A simple and multi-purpose process injector. 

Currently it supports injecting a DLL in two ways:

**Before the process starts** - This method creates the process in a suspended state and injects your DLL. 
This is helpful if you need to hook code early, for example if you need to capture/modify network packets.

![image](https://github.com/vektorprime/VekProcessInjector/assets/9269666/11e30513-e20b-44f2-b8c3-617d8cf209a2)


**After the process starts** - The more traditional way, which does not suspend the remote process. 
It just creates a new thread in that process to load your DLL.

![image](https://github.com/vektorprime/VekProcessInjector/assets/9269666/40314064-2fae-45d5-960f-0babb5f389b4)




**Examples**

Here's an example where I injected a DLL called bad.dll into Notepad running at PID 13884. The DLL injection generates a messagebox. The DLL is injected after the process has started.

![image](https://github.com/vektorprime/VekProcessInjector/assets/9269666/035916c2-fab4-4ed6-bb27-fd54eb9b72e9)

Here is Process Explorer showing the DLL loaded by Notepad.

![image](https://github.com/vektorprime/VekProcessInjector/assets/9269666/96e53e5d-832c-42f5-b351-6c875b16512d)


Here's the code for the DLL injected:

![image](https://github.com/vektorprime/VekProcessInjector/assets/9269666/d6408806-2edb-48ad-9837-fa6e12d7f4d3)
