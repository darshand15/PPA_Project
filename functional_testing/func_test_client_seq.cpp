#include "header_par_db_ft.h"
#include <chrono>
#include <fstream>

// Person: id, first name, last name, age, country, salary

int main()
{
    CREATE_TABLE(Person,
        ((int, id))
        ((std::string, fname))
        ((std::string, lname))
        ((int, age))
        ((std::string, country))
        ((int, salary))
    )

    SET_PRIMARY_KEY(Person, 
        ((id)))


        INSERT_INTO(Person, ((id, 91)) ((fname, "Mary")) ((lname, "Johnson")) ((age, 29)) ((country, "Greece")) ((salary, 191)))
        INSERT_INTO(Person, ((id, 43)) ((fname, "Carol")) ((lname, "Baker")) ((age, 61)) ((country, "Bulgaria")) ((salary, 143)))
        INSERT_INTO(Person, ((id, 34)) ((fname, "Paul")) ((lname, "Wright")) ((age, 52)) ((country, "Austria")) ((salary, 134)))
        INSERT_INTO(Person, ((id, 22)) ((fname, "Daniel")) ((lname, "Thompson")) ((age, 40)) ((country, "Finland")) ((salary, 122)))
        INSERT_INTO(Person, ((id, 80)) ((fname, "Benjamin")) ((lname, "Watson")) ((age, 18)) ((country, "Cyprus")) ((salary, 180)))
        INSERT_INTO(Person, ((id, 90)) ((fname, "James")) ((lname, "Smith")) ((age, 28)) ((country, "Ghana")) ((salary, 190)))
        INSERT_INTO(Person, ((id, 9)) ((fname, "Elizabeth")) ((lname, "Martinez")) ((age, 27)) ((country, "Botswana")) ((salary, 109)))
        INSERT_INTO(Person, ((id, 84)) ((fname, "Gregory")) ((lname, "James")) ((age, 22)) ((country, "Ethiopia")) ((salary, 184)))
        INSERT_INTO(Person, ((id, 100)) ((fname, "David")) ((lname, "Hernandez")) ((age, 38)) ((country, "Bangladesh")) ((salary, 200)))
        INSERT_INTO(Person, ((id, 15)) ((fname, "Jessica")) ((lname, "Thomas")) ((age, 33)) ((country, "Cuba")) ((salary, 115)))
        INSERT_INTO(Person, ((id, 78)) ((fname, "Brandon")) ((lname, "Ward")) ((age, 96)) ((country, "China")) ((salary, 178)))
        INSERT_INTO(Person, ((id, 99)) ((fname, "Elizabeth")) ((lname, "Martinez")) ((age, 37)) ((country, "Bahrain")) ((salary, 199)))
        INSERT_INTO(Person, ((id, 69)) ((fname, "Helen")) ((lname, "Cooper")) ((age, 87)) ((country, "Barbados")) ((salary, 169)))
        INSERT_INTO(Person, ((id, 11)) ((fname, "Barbara")) ((lname, "Lopez")) ((age, 29)) ((country, "Bulgaria")) ((salary, 111)))
        INSERT_INTO(Person, ((id, 79)) ((fname, "Emma")) ((lname, "Richardson")) ((age, 97)) ((country, "Cuba")) ((salary, 179)))
        INSERT_INTO(Person, ((id, 38)) ((fname, "Joshua")) ((lname, "Hill")) ((age, 56)) ((country, "Belarus")) ((salary, 138)))
        INSERT_INTO(Person, ((id, 56)) ((fname, "Jeffrey")) ((lname, "Cruz")) ((age, 74)) ((country, "Georgia")) ((salary, 156)))
        INSERT_INTO(Person, ((id, 40)) ((fname, "Kenneth")) ((lname, "Green")) ((age, 58)) ((country, "Bhutan")) ((salary, 140)))
        INSERT_INTO(Person, ((id, 59)) ((fname, "Cynthia")) ((lname, "Reyes")) ((age, 77)) ((country, "Greece")) ((salary, 159)))
        INSERT_INTO(Person, ((id, 93)) ((fname, "Patricia")) ((lname, "Brown")) ((age, 31)) ((country, "Hungary")) ((salary, 193)))
        INSERT_INTO(Person, ((id, 61)) ((fname, "Kathleen")) ((lname, "Morris")) ((age, 79)) ((country, "Hungary")) ((salary, 161)))
        INSERT_INTO(Person, ((id, 45)) ((fname, "Amanda")) ((lname, "Rivera")) ((age, 63)) ((country, "Canada")) ((salary, 145)))
        INSERT_INTO(Person, ((id, 49)) ((fname, "Deborah")) ((lname, "Roberts")) ((age, 67)) ((country, "Denmark")) ((salary, 149)))
        INSERT_INTO(Person, ((id, 33)) ((fname, "Kimberly")) ((lname, "King")) ((age, 51)) ((country, "Australia")) ((salary, 133)))
        INSERT_INTO(Person, ((id, 89)) ((fname, "Rachel")) ((lname, "Hughes")) ((age, 27)) ((country, "Germany")) ((salary, 189)))
        INSERT_INTO(Person, ((id, 74)) ((fname, "Justin")) ((lname, "Howard")) ((age, 92)) ((country, "Brazil")) ((salary, 174)))
        INSERT_INTO(Person, ((id, 29)) ((fname, "Sandra")) ((lname, "Robinson")) ((age, 47)) ((country, "Hungary")) ((salary, 129)))
        INSERT_INTO(Person, ((id, 30)) ((fname, "Donald")) ((lname, "Walker")) ((age, 48)) ((country, "USA")) ((salary, 130)))
        INSERT_INTO(Person, ((id, 94)) ((fname, "Robert")) ((lname, "Jones")) ((age, 32)) ((country, "USA")) ((salary, 194)))
        INSERT_INTO(Person, ((id, 23)) ((fname, "Lisa")) ((lname, "White")) ((age, 41)) ((country, "France")) ((salary, 123)))
        INSERT_INTO(Person, ((id, 10)) ((fname, "David")) ((lname, "Hernandez")) ((age, 28)) ((country, "Brazil")) ((salary, 110)))
        INSERT_INTO(Person, ((id, 1)) ((fname, "Mary")) ((lname, "Johnson")) ((age, 19)) ((country, "Australia")) ((salary, 101)))
        INSERT_INTO(Person, ((id, 12)) ((fname, "Richard")) ((lname, "Gonzalez")) ((age, 30)) ((country, "Cambodia")) ((salary, 112)))
        INSERT_INTO(Person, ((id, 96)) ((fname, "Michael")) ((lname, "Miller")) ((age, 34)) ((country, "Argentina")) ((salary, 196)))
        INSERT_INTO(Person, ((id, 13)) ((fname, "Susan")) ((lname, "Wilson")) ((age, 31)) ((country, "Canada")) ((salary, 113)))
        INSERT_INTO(Person, ((id, 68)) ((fname, "Stephen")) ((lname, "Morgan")) ((age, 86)) ((country, "Bangladesh")) ((salary, 168)))
        INSERT_INTO(Person, ((id, 36)) ((fname, "Andrew")) ((lname, "Torres")) ((age, 54)) ((country, "Bangladesh")) ((salary, 136)))
        INSERT_INTO(Person, ((id, 4)) ((fname, "Robert")) ((lname, "Jones")) ((age, 22)) ((country, "Bangladesh")) ((salary, 104)))
        INSERT_INTO(Person, ((id, 51)) ((fname, "Stephanie")) ((lname, "Phillips")) ((age, 69)) ((country, "Estonia")) ((salary, 151)))
        INSERT_INTO(Person, ((id, 20)) ((fname, "Christopher")) ((lname, "Lee")) ((age, 38)) ((country, "Ethiopia")) ((salary, 120)))
        INSERT_INTO(Person, ((id, 18)) ((fname, "Charles")) ((lname, "Jackson")) ((age, 36)) ((country, "Egypt")) ((salary, 118)))
        INSERT_INTO(Person, ((id, 85)) ((fname, "Christine")) ((lname, "Bennett")) ((age, 23)) ((country, "Fiji")) ((salary, 185)))
        INSERT_INTO(Person, ((id, 35)) ((fname, "Emily")) ((lname, "Scott")) ((age, 53)) ((country, "Bahrain")) ((salary, 135)))
        INSERT_INTO(Person, ((id, 71)) ((fname, "Anna")) ((lname, "Bailey")) ((age, 89)) ((country, "Belgium")) ((salary, 171)))
        INSERT_INTO(Person, ((id, 24)) ((fname, "Matthew")) ((lname, "Harris")) ((age, 42)) ((country, "Georgia")) ((salary, 124)))
        INSERT_INTO(Person, ((id, 31)) ((fname, "Ashley")) ((lname, "Young")) ((age, 49)) ((country, "India")) ((salary, 131)))
        INSERT_INTO(Person, ((id, 3)) ((fname, "Patricia")) ((lname, "Brown")) ((age, 21)) ((country, "Bahrain")) ((salary, 103)))
        INSERT_INTO(Person, ((id, 21)) ((fname, "Nancy")) ((lname, "Perez")) ((age, 39)) ((country, "Fiji")) ((salary, 121)))
        INSERT_INTO(Person, ((id, 65)) ((fname, "Shirley")) ((lname, "Rogers")) ((age, 83)) ((country, "Australia")) ((salary, 165)))
        INSERT_INTO(Person, ((id, 39)) ((fname, "Michelle")) ((lname, "Flores")) ((age, 57)) ((country, "Belgium")) ((salary, 139)))
        INSERT_INTO(Person, ((id, 54)) ((fname, "Jason")) ((lname, "Diaz")) ((age, 72)) ((country, "Finland")) ((salary, 154)))
        INSERT_INTO(Person, ((id, 83)) ((fname, "Katherine")) ((lname, "Wood")) ((age, 21)) ((country, "Estonia")) ((salary, 183)))
        INSERT_INTO(Person, ((id, 41)) ((fname, "Dorothy")) ((lname, "Adams")) ((age, 59)) ((country, "Botswana")) ((salary, 141)))
        INSERT_INTO(Person, ((id, 27)) ((fname, "Margaret")) ((lname, "Ramirez")) ((age, 45)) ((country, "Greece")) ((salary, 127)))
        INSERT_INTO(Person, ((id, 75)) ((fname, "Pamela")) ((lname, "Ramos")) ((age, 93)) ((country, "Bulgaria")) ((salary, 175)))
        INSERT_INTO(Person, ((id, 44)) ((fname, "Brian")) ((lname, "Hall")) ((age, 62)) ((country, "Cambodia")) ((salary, 144)))
        INSERT_INTO(Person, ((id, 60)) ((fname, "Jacob")) ((lname, "Stewart")) ((age, 78)) ((country, "Guyana")) ((salary, 160)))
        INSERT_INTO(Person, ((id, 26)) ((fname, "Anthony")) ((lname, "Clark")) ((age, 44)) ((country, "Ghana")) ((salary, 126)))
        INSERT_INTO(Person, ((id, 73)) ((fname, "Brenda")) ((lname, "Kelly")) ((age, 91)) ((country, "Botswana")) ((salary, 173)))
        INSERT_INTO(Person, ((id, 82)) ((fname, "Samuel")) ((lname, "Chavez")) ((age, 20)) ((country, "Egypt")) ((salary, 182)))
        INSERT_INTO(Person, ((id, 70)) ((fname, "Jonathan")) ((lname, "Peterson")) ((age, 88)) ((country, "Belarus")) ((salary, 170)))
        INSERT_INTO(Person, ((id, 46)) ((fname, "George")) ((lname, "Campbell")) ((age, 64)) ((country, "China")) ((salary, 146)))
        INSERT_INTO(Person, ((id, 37)) ((fname, "Donna")) ((lname, "Nguyen")) ((age, 55)) ((country, "Barbados")) ((salary, 137)))
        INSERT_INTO(Person, ((id, 58)) ((fname, "Ryan")) ((lname, "Collins")) ((age, 76)) ((country, "Ghana")) ((salary, 158)))
        INSERT_INTO(Person, ((id, 52)) ((fname, "Timothy")) ((lname, "Evans")) ((age, 70)) ((country, "Ethiopia")) ((salary, 152)))
        INSERT_INTO(Person, ((id, 81)) ((fname, "Samantha")) ((lname, "Brooks")) ((age, 19)) ((country, "Denmark")) ((salary, 181)))
        INSERT_INTO(Person, ((id, 88)) ((fname, "Alexander")) ((lname, "Ruiz")) ((age, 26)) ((country, "Georgia")) ((salary, 188)))
        INSERT_INTO(Person, ((id, 48)) ((fname, "Edward")) ((lname, "Carter")) ((age, 66)) ((country, "Cyprus")) ((salary, 148)))
        INSERT_INTO(Person, ((id, 53)) ((fname, "Rebecca")) ((lname, "Turner")) ((age, 71)) ((country, "Fiji")) ((salary, 153)))
        INSERT_INTO(Person, ((id, 67)) ((fname, "Angela")) ((lname, "Ortiz")) ((age, 85)) ((country, "Bahrain")) ((salary, 167)))
        INSERT_INTO(Person, ((id, 57)) ((fname, "Sharon")) ((lname, "Edwards")) ((age, 75)) ((country, "Germany")) ((salary, 157)))
        INSERT_INTO(Person, ((id, 42)) ((fname, "Kevin")) ((lname, "Nelson")) ((age, 60)) ((country, "Brazil")) ((salary, 142)))
        INSERT_INTO(Person, ((id, 19)) ((fname, "Karen")) ((lname, "Martin")) ((age, 37)) ((country, "Estonia")) ((salary, 119)))
        INSERT_INTO(Person, ((id, 66)) ((fname, "Eric")) ((lname, "Gutierrez")) ((age, 84)) ((country, "Austria")) ((salary, 166)))
        INSERT_INTO(Person, ((id, 6)) ((fname, "Michael")) ((lname, "Miller")) ((age, 24)) ((country, "Belarus")) ((salary, 106)))
        INSERT_INTO(Person, ((id, 14)) ((fname, "Joseph")) ((lname, "Anderson")) ((age, 32)) ((country, "China")) ((salary, 114)))
        INSERT_INTO(Person, ((id, 98)) ((fname, "William")) ((lname, "Rodriguez")) ((age, 36)) ((country, "Austria")) ((salary, 198)))
        INSERT_INTO(Person, ((id, 62)) ((fname, "Gary")) ((lname, "Morales")) ((age, 80)) ((country, "USA")) ((salary, 162)))
        INSERT_INTO(Person, ((id, 87)) ((fname, "Debra")) ((lname, "Mendoza")) ((age, 25)) ((country, "France")) ((salary, 187)))
        INSERT_INTO(Person, ((id, 8)) ((fname, "William")) ((lname, "Rodriguez")) ((age, 26)) ((country, "Bhutan")) ((salary, 108)))
        INSERT_INTO(Person, ((id, 95)) ((fname, "Jennifer")) ((lname, "Garcia")) ((age, 33)) ((country, "India")) ((salary, 195)))
        INSERT_INTO(Person, ((id, 72)) ((fname, "Larry")) ((lname, "Reed")) ((age, 90)) ((country, "Bhutan")) ((salary, 172)))
        INSERT_INTO(Person, ((id, 50)) ((fname, "Ronald")) ((lname, "Gomez")) ((age, 68)) ((country, "Egypt")) ((salary, 150)))
        INSERT_INTO(Person, ((id, 86)) ((fname, "Frank")) ((lname, "Gray")) ((age, 24)) ((country, "Finland")) ((salary, 186)))
        INSERT_INTO(Person, ((id, 63)) ((fname, "Amy")) ((lname, "Murphy")) ((age, 81)) ((country, "India")) ((salary, 163)))
        INSERT_INTO(Person, ((id, 7)) ((fname, "Linda")) ((lname, "Davis")) ((age, 25)) ((country, "Belgium")) ((salary, 107)))
        INSERT_INTO(Person, ((id, 92)) ((fname, "John")) ((lname, "Williams")) ((age, 30)) ((country, "Guyana")) ((salary, 192)))
        INSERT_INTO(Person, ((id, 2)) ((fname, "John")) ((lname, "Williams")) ((age, 20)) ((country, "Austria")) ((salary, 102)))
        INSERT_INTO(Person, ((id, 17)) ((fname, "Sarah")) ((lname, "Moore")) ((age, 35)) ((country, "Denmark")) ((salary, 117)))
        INSERT_INTO(Person, ((id, 64)) ((fname, "Nicholas")) ((lname, "Cook")) ((age, 82)) ((country, "Argentina")) ((salary, 164)))
        INSERT_INTO(Person, ((id, 5)) ((fname, "Jennifer")) ((lname, "Garcia")) ((age, 23)) ((country, "Barbados")) ((salary, 105)))
        INSERT_INTO(Person, ((id, 55)) ((fname, "Laura")) ((lname, "Parker")) ((age, 73)) ((country, "France")) ((salary, 155)))
        INSERT_INTO(Person, ((id, 77)) ((fname, "Nicole")) ((lname, "Cox")) ((age, 95)) ((country, "Canada")) ((salary, 177)))
        INSERT_INTO(Person, ((id, 47)) ((fname, "Melissa")) ((lname, "Mitchell")) ((age, 65)) ((country, "Cuba")) ((salary, 147)))
        INSERT_INTO(Person, ((id, 25)) ((fname, "Betty")) ((lname, "Sanchez")) ((age, 43)) ((country, "Germany")) ((salary, 125)))
        INSERT_INTO(Person, ((id, 76)) ((fname, "Scott")) ((lname, "Kim")) ((age, 94)) ((country, "Cambodia")) ((salary, 176)))
        INSERT_INTO(Person, ((id, 28)) ((fname, "Mark")) ((lname, "Lewis")) ((age, 46)) ((country, "Guyana")) ((salary, 128)))
        INSERT_INTO(Person, ((id, 16)) ((fname, "Thomas")) ((lname, "Taylor")) ((age, 34)) ((country, "Cyprus")) ((salary, 116)))
        INSERT_INTO(Person, ((id, 97)) ((fname, "Linda")) ((lname, "Davis")) ((age, 35)) ((country, "Australia")) ((salary, 197)))
        INSERT_INTO(Person, ((id, 32)) ((fname, "Steven")) ((lname, "Allen")) ((age, 50)) ((country, "Argentina")) ((salary, 132)))
    
    std::cout << "Select PK EQ\n";
    SELECT_PK_EQ_SEQ(Person, id, ==, 17)
    
    std::cout << "Select PK Range \n";
    SELECT_ALL_RANGE(Person, id, 13, 47)
    
    std::cout << "Select Cols Cond \n";
    SELECT_COLS_COND(Person,
        ((id))
        ((fname))
        ((age)),
        age, >=, 25
    )
    
    std::cout << "Select All Cond \n";
    SELECT_ALL_COND(Person, salary, >=, 25)
    
    std::cout << "Order by Asc \n";
    ORDER_BY_ASC(Person,
        ((fname))
        ((id))
    )
    
    std::cout << "Order by Desc \n";
    ORDER_BY_DESC(Person,
        ((salary))
        ((id))
    )
    
    std::cout << "Group By Count \n";
    GROUP_BY_COUNT(Person,
        ((lname))
    )
    
    std::cout << "Group By Min \n";
    GROUP_BY_MIN(Person,
        ((country)), salary
    )
    
    std::cout << "Group By Max \n";
    GROUP_BY_MAX(Person,
        ((country)), age
    )
    
    std::cout << "Group By Sum \n";
    GROUP_BY_SUM(Person,
        ((country)), salary
    )
    
    std::cout << "Group By Avg \n";
    GROUP_BY_AVG(Person,
        ((country)), salary
    )

DELETE_ALL_ROWS(Person)

return 0;
}