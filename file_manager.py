
class FileManager:
    def __init__(self, file_name):
        self.file_name = file_name

    def read_file(self):
        try:
            return open(self.file_name, encoding='utf-8').read()
        except IOError:
            return "Nie udalo sie odczytac pliku"
    def update_file(self, text_data):
        try:
            val = open(self.file_name,'a');
            val.write(text_data)
            val.close()
        except IOError:
            return "cos poszlo nie tak"


        