# VekProcessInjector

A simple and multi-purpose process injector. 

Currently it supports injecting a DLL in two ways:
Before the process starts - This method creates the process in a suspended state and injects your DLL. 
This is helpful if you need to hook code early, for example if you need to capture/modify network packets.

![image](https://github.com/vektorprime/VekProcessInjector/assets/9269666/11e30513-e20b-44f2-b8c3-617d8cf209a2)


After the process starts - The more traditional way, which does not suspend the remote process. 
It just creates a new thread in that process to load your DLL.

![image](https://github.com/vektorprime/VekProcessInjector/assets/9269666/40314064-2fae-45d5-960f-0babb5f389b4)


