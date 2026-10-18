from VendingMachine import VendingMachine

if __name__ == "__main__":
    Machine = VendingMachine()
    Machine.Start()
    while True:
        try:
            Machine.work()
        except ValueError as error:
            print("Error: Invalid input value. Please try again.")
        except:
            print("Ops, something went wrong. Try again")
            quit()