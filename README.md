# 🎵 MP3 Tag Reader

A C project to read ID3 tags (Title, Artist, Album, Year, etc.) from MP3 files. The **MP3 Tag Reader** is a command-line utility written in C that extracts metadata from MP3 audio files.  
It parses the **ID3v1** tag format and displays information such as **Title, Artist, Album, Year, Genre, and Comment**.

---

## ⚡ Features
- Reads ID3v1 tags from MP3 files
- Displays metadata in a clean tabular format
- Handles missing/invalid tags gracefully
- Lightweight and portable (written in pure C)

---

##  Prerequisites
- **Language:** C programming  
- File I/O operations  
- String handling and memory management  
- Pointers and function pointers :contentReference[oaicite:7]{index=7}
- **Compiler:** GCC

---

## 📌 Applications
- Music Library Organizers
- Music Streaming Services
- Car Infotainment Systems


---

## 🖥️ Usage

### 1. View MP3 Tags
```bash
./a.out -v <mp3filename>
```

### 2. Edit MP3 Tags
``` bash
./a.out -e <Tag> <editing_text> <mp3filename>
```
🎯 Tags: 
- t → Title
- a → Artist
- A → Album
- m → Music/Genre
- y → Year
- c → Comment
