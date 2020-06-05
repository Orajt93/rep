from file_manager import FileManager
#1
print("Zadanie 1")

a_list = [1, 2, 3, 4, 5, 6]
b_list = [10, 20, 30, 40, 50, 60, 70, 80]

def funkcja1(a_list, b_list):
    zad1return=[]
    if(len(a_list)>len(b_list)):
        liczbaIteracji=len(a_list)
    else:
        liczbaIteracji=len(b_list)
    for i in range(0,liczbaIteracji):
        if(i<len(a_list) and i%2==0):
            zad1return.append(i)
        if(i<len(b_list) and i%2==1):
            zad1return.append(i)
    return zad1return
#print(funkcja1(a_list,b_list))

#2
print("Zadanie 2")
def funkcja2(data_text):
    zad2return={
        "length":len(data_text),
        "letters": list(data_text),
        "big_letters" : data_text.upper(),
        "small_letters": data_text.lower()}
    return zad2return
#print(funkcja2("Jestem glodny"))

#3
print("Zadanie 3")
def funkcja3(text, letter):
    return text.replace(letter.upper(),"").replace(letter.lower(),"")
#print(funkcja3("NiE wiem",'e'))

#4
print("Zadanie 4")
def funkcja4(stCelcius, temperature_type):
    if(stCelcius < -273.15):
        return "podano bledna temperature"
    if(temperature_type=="fahrenheit"):
        return "{0} stopni celcjusza to {1} fahrenheit'a".format(stCelcius,stCelcius*1.8+32)
    if(temperature_type=="rankine"):
        return "{0} stopni celcjusza to {1} rankine'a".format(stCelcius,(stCelcius+273.15)*1.8)
    if(temperature_type=="kelvin"):
        return "{0} stopni celcjusza to {1} kelvin'a".format(stCelcius,stCelcius+273.15)
    return "podano zly typ wyjsciowy, dostepne typy: fahrenheit, rankine, kelvin"

#print(funkcja4(20,"fahrenheit"))
#print(funkcja4(20,"rankine"))
#print(funkcja4(20,"random"))

#5
print("Zadanie 5")
class Calculator:
    def add(l1,l2):
        return l1 + l1

    def difference(l1,l2):
        return l1 - l2

    def multiply(l1,l2):
        return l1 * l2

    def divide(l1,l2):
        if l2 != 0:
            return l1 / l2
        return "Nie da sie dzielic przez 0"

#print(Calculator.divide(4,2))

#6
print("Zadanie 6")
class ScienceCalculator(Calculator):
    def pow(l1,l2):
        return l1**l2
#print(ScienceCalculator.add(2,3))
#print(ScienceCalculator.pow(2,3))

#7
print("Zadanie 7")
def funkcja7(tekst):
    return tekst[::-1]
#print(funkcja7("kotel"))

#8/9
zad9=FileManager("tekst.txt")
print(zad9.read_file())
zad9.update_file(" dodoaje tekst")
print(zad9.read_file())

#print(zad9.read_file())


