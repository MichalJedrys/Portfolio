import time

class Display():
    def Start(self):
        print("Vending Machine is starting .",end=''); time.sleep(0.5)
        print('.',end=''); time.sleep(0.5)
        print('.\n'); time.sleep(0.5)

    def MainMenu(self,menu):
        nr=1
        for x in menu:
            print (nr , x[1] , "\t\t\t" , x[0] , '$')
            nr+=1
        print ('11. Maintenance mode\n')
        n = input('Pick an option: ')
        
        if int(n) < 1 or int(n) > 11:
            raise ValueError("Invalid value.")
        return int(n)

    def MaintenanceMenu(self,passwd):
        p = input("Enter a password: ")
        if p == passwd:
            print("""\n     Maintenance MENU     \n 
            1. History of transactions\n
            2. Money withdraw         \n
            3. Turn off the machine   \n
            4. Return to main menu    \n""")
            n = input("Pick an option: ")
            if int(n) < 1 or int(n) > 4:
                raise ValueError("Invalid value.")
            return int(n)
        else:
            print('\nWrong password. Exiting to main menu...')

    def PrepDisp(self,item):
        print("\nPreparing " + item[1] + ".",end=''); time.sleep(0.5)
        print('.',end=''); time.sleep(0.5)
        print('.',end=''); time.sleep(0.5)
        print('.',end=''); time.sleep(0.5)
        print('.'); time.sleep(0.5)
        print('\n' + item[1] + " is ready.\n"); time.sleep(2)