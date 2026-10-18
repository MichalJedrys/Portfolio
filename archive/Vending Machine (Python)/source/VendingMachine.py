from Feeder import Feeder
from Display import Display
from Controller import Controller

class VendingMachine(Display,Controller,Feeder):
    def __init__(self):
            self.__option = -1
            self.__moption = -1
            self.__product = ()
            super().__init__()

    def work(self):
        self.__option = super().MainMenu(super().GetMenu())
        if self.__option == 11:
            self.__moption = super().MaintenanceMenu(super().GetPassword())
            if self.__moption == 0: 
                return
            else:
                super().SelectMOption(self.__moption)
        else:
            self.__product = super().SelectBev(self.__option)
            if super().PaymentProcessing(self.__product):
                if self.__option < 6:
                    super().MilkAndSugar(self.__option)
                else:
                    super().SoySauce()
                super().PrepDisp(self.__product)