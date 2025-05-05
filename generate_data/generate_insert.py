import random

max_id = 10000

first_names = [
    "James", "Mary", "John", "Patricia", "Robert", "Jennifer", "Michael", "Linda", "William", "Elizabeth",
    "David", "Barbara", "Richard", "Susan", "Joseph", "Jessica", "Thomas", "Sarah", "Charles", "Karen",
    "Christopher", "Nancy", "Daniel", "Lisa", "Matthew", "Betty", "Anthony", "Margaret", "Mark", "Sandra",
    "Donald", "Ashley", "Steven", "Kimberly", "Paul", "Emily", "Andrew", "Donna", "Joshua", "Michelle",
    "Kenneth", "Dorothy", "Kevin", "Carol", "Brian", "Amanda", "George", "Melissa", "Edward", "Deborah",
    "Ronald", "Stephanie", "Timothy", "Rebecca", "Jason", "Laura", "Jeffrey", "Sharon", "Ryan", "Cynthia",
    "Jacob", "Kathleen", "Gary", "Amy", "Nicholas", "Shirley", "Eric", "Angela", "Stephen", "Helen",
    "Jonathan", "Anna", "Larry", "Brenda", "Justin", "Pamela", "Scott", "Nicole", "Brandon", "Emma",
    "Benjamin", "Samantha", "Samuel", "Katherine", "Gregory", "Christine", "Frank", "Debra", "Alexander", "Rachel"
]

last_names = [
    "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis", "Rodriguez", "Martinez",
    "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson", "Thomas", "Taylor", "Moore", "Jackson", "Martin",
    "Lee", "Perez", "Thompson", "White", "Harris", "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson",
    "Walker", "Young", "Allen", "King", "Wright", "Scott", "Torres", "Nguyen", "Hill", "Flores",
    "Green", "Adams", "Nelson", "Baker", "Hall", "Rivera", "Campbell", "Mitchell", "Carter", "Roberts",
    "Gomez", "Phillips", "Evans", "Turner", "Diaz", "Parker", "Cruz", "Edwards", "Collins", "Reyes",
    "Stewart", "Morris", "Morales", "Murphy", "Cook", "Rogers", "Gutierrez", "Ortiz", "Morgan", "Cooper",
    "Peterson", "Bailey", "Reed", "Kelly", "Howard", "Ramos", "Kim", "Cox", "Ward", "Richardson",
    "Watson", "Brooks", "Chavez", "Wood", "James", "Bennett", "Gray", "Mendoza", "Ruiz", "Hughes"
]

countries = [
    "Argentina", "Australia", "Austria", "Bahrain", "Bangladesh",
    "Barbados", "Belarus", "Belgium", "Bhutan", "Botswana",
    "Brazil", "Bulgaria", "Cambodia", "Canada", "China",
    "Cuba", "Cyprus", "Denmark", "Egypt", "Estonia",
    "Ethiopia", "Fiji", "Finland", "France", "Georgia",
    "Germany", "Ghana", "Greece", "Guyana", "Hungary", "USA", "India"
]

ids = list(range(1, max_id+1))
random.shuffle(ids)

with open("test_client.cpp", "w") as file:
    for i in range(max_id):
        id = ids[i]
        fname = first_names[(ids[i])%len(first_names)]
        lname = last_names[(ids[i])%len(last_names)]
        age = (ids[i])%80 + 18
        country = countries[(ids[i])%len(countries)]
        salary = (ids[i])%(200000 - 100) + 100

        file.write(f"INSERT_INTO(Person, ((id, {id})) ((fname, \"{fname}\")) ((lname, \"{lname}\")) ((age, {age})) ((country, \"{country}\")) ((salary, {salary})))\n")


