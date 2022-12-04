# CMPE 279 Assignment 4
## Members
- Shereen Punnassery
- Mu-Te Shen

## Questions
We choose to do the stored XSS attack
1. The attack we used is enter the following string in message:
```
<a href="https://www.google.com">Click ME</a>
```
This will be rendered as a clickable link in the message. This is just to demonstrate the the website render whatever message a user enters. 

2. On **Medium**, the trick above is not working anymore. The website removed all html tags and only preserved string.

3. On **Low**, the website only strip slashes on *message*, and did not do any input sanitization on *name*. This leaves it vulnerable to all kinds of html/Javascript types of attacks. On **High**, they sanitize both the *message* and the *name* with more advance technique. This includes *strip_tags*, *htmlspecialchars*, and a regex that replace anything that'll result in a *\<script>*.  
