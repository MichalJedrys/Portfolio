import time
from decimal import Decimal, InvalidOperation

class Controller():
    def __init__(self):
        self.__password = "HardPassword"
        self.__MoneySum = Decimal("0")
        self.__PaymentHistory = []
        self.__Menu_Items = (
                (Decimal("2.50"),"Coffee"),
                (Decimal("1.50"),"Black Tea"),
                (Decimal("1.50"),"Green Tea"),
                (Decimal("2.00"),"Cocoa"),
                (Decimal("1.00"),"Hot Water"),
                (Decimal("3.00"),"Chicken Soup"),
                (Decimal("3.00"),"Tomato Soup"),
                (Decimal("3.00"),"Borscht"),
                (Decimal("3.50"),"Miso Soup"),
                (Decimal("3.00"),"Onion Soup"))

    def PaymentProcessing(self,item):
        print("""\n  Vending machine accepts only:  \n
                    0.25$, 0.5$, 1$, 2$, 5$ coins.  \n
                    The machine gives change.  \n""")
        rest = item[0]
        accepted_coins = {Decimal(value) for value in ("0", "0.25", "0.5", "1", "2", "5")}

        def cancel_payment():
            print("Payment cancelled.")
            if rest < item[0]:
                print(f"Refunding {item[0] - rest:.2f}$")
            return False

        while rest > 0:
            print('Enter 0 to cancel payment and return inserted coins.')
            try:
                payment = Decimal(input("Insert coin: "))
            except EOFError:
                return cancel_payment()
            except InvalidOperation:
                print("Invalid coin. Please try again.")
                continue
            if not payment.is_finite() or payment not in accepted_coins:
                print("Invalid coin. Please try again.")
                continue
            if payment == 0:
                return cancel_payment()
            rest-=payment
        self.__MoneySum+=item[0]
        self.__PaymentHistory.append(item)
        if rest < 0:
            print('\nGiving change .',end=''); time.sleep(0.5)
            print('.',end=''); time.sleep(0.5)
            print('.'); time.sleep(0.5)
            print('\nYour change is ' , -rest , '$')
        return True

    def SelectMOption(self,mopt):
        match mopt:
            case 1:
                for x in self.__PaymentHistory:
                    print(x[0] ,' - ', x[1])
                print('\nReturning to maintenance menu...')
                input('Press any key to continue.')
            case 2:
                print('\nCollected: ', self.__MoneySum ,'$\n\nReturning to maintenance menu...')
                input('Press any key to continue.')
                self.__MoneySum = Decimal("0")
            case 3:
                print('Vending Machine is shutting down .',end=''); time.sleep(0.5)
                print('.',end=''); time.sleep(0.5)
                print('.'); time.sleep(0.5)
                exit(0)
            case 4:
                pass

    def SelectBev(self,option):
        return self.__Menu_Items[option-1]

    def GetMenu(self):
        return self.__Menu_Items

    def GetPassword(self):
        return self.__password