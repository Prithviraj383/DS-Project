# Campus Complaint Registration & Resolution System

A simple **C language project** that simulates a real-world campus/hostel complaint handling system using **core Data Structures** such as **Linked Lists, Queues, Stacks, and Arrays**.

This project focuses on **clarity of design**, **proper use of data structures**, and **fair complaint resolution (FIFO)** rather than unnecessary complexity.

---

## 📌 Problem Statement

In a campus or hostel environment, students often raise complaints related to WiFi, electricity, plumbing, cleanliness, etc.  
Managing these complaints manually can lead to delays, unfair handling, and lack of proper tracking.

This project implements a **Complaint Registration & Resolution System** that:
- Registers complaints with unique ticket IDs
- Ensures complaints are resolved in the order they are received
- Maintains complaint history
- Supports reopening recently resolved complaints

---

## 🎯 Objectives

- To simulate a real-world ticketing/helpdesk system
- To demonstrate effective usage of fundamental data structures
- To ensure **fair complaint processing using FIFO**
- To keep the system simple, modular, and easy to understand

---

## 🧠 Data Structures Used

| Data Structure | Purpose |
|---------------|---------|
| **Linked List** | Stores all complaint records dynamically |
| **Queue** | Maintains FIFO order of pending complaints |
| **Stack** | Stores recently resolved complaints for undo/reopen |
| **Array** | Stores fixed complaint categories |

---

## 🏗️ System Design Overview

### 1️⃣ Linked List (Complaint Database)
- Stores every complaint ever registered
- Complaints are inserted using **tail insertion (FIFO)** for clarity
- Complaints are **never deleted**, only their status is updated

### 2️⃣ Queue (Pending Complaints)
- Stores **only complaint IDs**
- Ensures the **oldest complaint is resolved first**
- Controls complaint processing order

### 3️⃣ Stack (Resolved Complaints)
- Stores IDs of resolved complaints
- Enables **undo/reopen of the most recently resolved complaint**

### 4️⃣ Array (Categories)
- Stores predefined complaint categories such as:
  - WiFi
  - Electricity
  - Plumbing
  - Cleanliness
  - Other

---

## ⚙️ Features

- Register a new complaint
- Auto-generate unique ticket IDs
- View all complaints (oldest first)
- Process complaints in FIFO order
- Mark complaints as resolved
- Reopen the last resolved complaint
- Simple menu-driven interface
