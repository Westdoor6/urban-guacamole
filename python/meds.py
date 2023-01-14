import sqlite3

##          Name        mg Acet 
HYDRO = '''INSERT INTO pill_history (name, acet_mg) VALUES ('Hydrocodone', 325);'''
TYLEN = '''INSERT INTO pill_history (name, acet_mg) VALUES ("Tylenol", 500);'''
PASTDAY  = '''SELECT acet_mg FROM pill_history WHERE log_time > datetime('now','-1 day')'''

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


def log_all(table):
    header = "Index     Medication      Acetaminophen       Time Taken"
    print(header)

    for row in table:
        index, name, mg, created_at = row
        print(f'{index}\t {name}\t {mg}\t\t {created_at}')

    print()

def log_acet(table):
    total = 0
    for mg in table:
        total += int(mg[0]) #MG has 1 value but is returned as a tuple from the database

    print("In the past 24 hours " + str(total) + " mg of acetaminophen have been logged")

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
        log_all(res.fetchall())
    elif m == 4:
        res = dbc.execute(PASTDAY)
        log_acet(res.fetchall())
    elif m == 5:
        dbc.close()
        con.close()
        quit()

