import sqlite3

##          Name        mg Acet 
HYDRO = '''INSERT INTO pill_history (name, acet_mg) VALUES ('Hydrocodone', 325);'''
TYLEN = '''INSERT INTO pill_history (name, acet_mg) VALUES ("Tylenol", 500);'''

def db_setup():
    ## Connect to our database of medication entries
    try:
        sqliteConnection = sqlite3.connect("meds.db")
        sqlite_create_table_query = '''CREATE TABLE pill_history (
                                    "id" INTEGER PRIMARY KEY,
                                    "name" TEXT NOT NULL,
                                    "acet_mg" INT NOT NULL,
                                    "log_time" DATETIME DEFAULT CURRENT_TIMESTAMP);'''

        cursor = sqliteConnection.cursor()
        print("Successfully Connected to SQLite")
        cursor.execute(sqlite_create_table_query)
        sqliteConnection.commit()
        print("SQLite table created")

        cursor.close()

    except sqlite3.Error as error:
        print("Error while creating a sqlite table", error)
    finally:
        if sqliteConnection:
            sqliteConnection.close()
            print("sqlite connection is closed")


def log(table):
    header = "Index     Medication      Acetaminophen       Time Taken"
    print(header)

    for row in table:
        index, name, mg, created_at = row
        print(f'{index}\t {name}\t {mg}\t\t {created_at}')

    print()

db_setup()
con = sqlite3.connect("meds.db")
dbc = con.cursor()

print("Welcome to meds.py, plesase enter the action you'd like to take:")
options = "1. Log Hydrocodone \n2. Log Tylenol \n3. View Medication History \n4. Quick Check Acet. \n5. Quit\n"
menu_choice = -1
while int(menu_choice) != 5:
    menu_choice = input(options)

    m = int(menu_choice)
    if m == 1:
        dbc.execute(HYDRO)
        con.commit()
        print()
    elif m == 2:
        dbc.execute(TYLEN)
        con.commit()
        print()
    elif m == 3:
        res = dbc.execute("SELECT * FROM pill_history")
        log(res.fetchall())
    elif m == 4:
        print()
    elif m == 5:
        dbc.close()
        con.close()
        quit()

