class Feeder():
    def SoySauce(self):
        c = input('\nDo you want to add soy sauce? (y/n)')
        if c != 'y' and c != 'n':
            raise ValueError("Invalid value.")
 
    def MilkAndSugar(self,ID):
        if ID==1:
            c = input('\nDo you want to add milk? (y/n)')
            if c != 'y' and c != 'n':
                raise ValueError("Invalid value.")
        n = input('\nSet amount of sugar you want (0-6): ')
        if int(n) < 0 or int(n) > 6:
            raise ValueError("Invalid value.")