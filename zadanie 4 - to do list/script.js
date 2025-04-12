document.getElementById("dodaj").addEventListener("click", () => {
  const list = document.getElementById("itemList");
  const inputField = document.getElementById("wpisz");
  const taskText = inputField.value.trim();

  if (taskText === "") {
    alert("Brak tekstu w nowym elemencie");
    return;
  }

  const listItem = document.createElement("li");

  const itemNumber = document.createElement("span");
  itemNumber.className = "item-number";

  const toggleButton = document.createElement("button");
  toggleButton.innerHTML = "◻";
  toggleButton.className = "toggle-btn";

  const taskSpan = document.createElement("span");
  taskSpan.textContent = taskText;
  taskSpan.className = "task-text";

  const deleteButton = document.createElement("button");
  deleteButton.textContent = "usuń";
  deleteButton.className = "delete-btn";

  toggleButton.addEventListener("click", () => {
    taskSpan.classList.toggle("completed");
    toggleButton.innerHTML = taskSpan.classList.contains("completed")
      ? "✓"
      : "◻";
  });

  deleteButton.addEventListener("click", () => {
    listItem.remove();
    updateItemNumbers();
  });

  listItem.appendChild(itemNumber);
  listItem.appendChild(toggleButton);
  listItem.appendChild(taskSpan);
  listItem.appendChild(deleteButton);

  list.appendChild(listItem);

  updateItemNumbers();

  inputField.value = "";
  inputField.focus();
});

document.getElementById("wpisz").addEventListener("keypress", (e) => {
  if (e.key === "Enter") {
    document.getElementById("dodaj").click();
  }
});

document.getElementById("usunZaz").addEventListener("click", () => {
  const completedItems = document.querySelectorAll("#itemList li .completed");
  completedItems.forEach((completedItem) => {
    completedItem.closest("li").remove();
  });
  updateItemNumbers();
});

function updateItemNumbers() {
  const items = document.querySelectorAll("#itemList li");
  items.forEach((item, index) => {
    const numberSpan = item.querySelector(".item-number");
    if (numberSpan) {
      numberSpan.textContent = index + 1 + ".";
    } else {
      const newNumber = document.createElement("span");
      newNumber.className = "item-number";
      newNumber.textContent = index + 1 + ".";
      item.insertBefore(newNumber, item.firstChild);
    }
  });
}
