// Verification Block: Check if we are running in Thunder JS Runtime or Node.js
if (typeof __raw_print_stdout !== 'undefined') {
    console.log("=== VERIFICATION: Running on [Thunder JS Runtime] ===");
} else {
    console.log("=== VERIFICATION: Running on [Node.js] ===");
}
console.log("");

// 1. Custom Quick Sort Implementation
Array.prototype.sorting = function(low, high, callback) {
    if (low >= high) return;

    let mid = Math.floor((low + high) / 2);

    this.sorting(low, mid, callback);
    this.sorting(mid + 1, high, callback);

    let temp = [];
    let l = low;
    let r = mid + 1;

    while (l <= mid && r <= high) {
        if (callback(this[l], this[r]) < 0) {
            temp.push(this[l]);
            l++;
        } else {
            temp.push(this[r]);
            r++;
        }
    }

    while (l <= mid) {
        temp.push(this[l]);
        l++;
    }

    while (r <= high) {
        temp.push(this[r]);
        r++;
    }

    for (let i = low; i <= high; i++)
        this[i] = temp[i - low];
};

const arr = [9, 2, 31, 4, 5];
arr.sorting(0, arr.length - 1, (a, b) => a - b);
console.log("Sorted Array:", arr);

// 2. Standard forEach
const arr2 = [10, 20, 30, 40];
console.log("Standard forEach:");
arr2.forEach((num) => console.log(num));

// 3. Custom forloop (forEach replacement)
// Tip: Using 'this' instead of 'arr2' makes it work for any array!
Array.prototype.forloop = function(callback) {
     for(let i = 0; i < this.length; i++){
          callback(this[i], i);
     }
}
console.log("Custom forloop:");
arr2.forloop((num, index) => { console.log(num, index); });

// 4. Standard filter
console.log("Standard filter (>= 30):", arr2.filter((a) => (a >= 30)));

// 5. Custom filter
Array.prototype.customfilter = function(callback) {
    let arr3 = [];
    for (let i of this) {
        if (callback(i)) arr3.push(i);
    }
    return arr3;
}
console.log("Custom customfilter (== 10):", arr2.customfilter((a) => (a == 10)));
