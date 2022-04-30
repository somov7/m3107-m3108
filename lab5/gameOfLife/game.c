#include "game.h"

int **mallocForArr(int height, int width){
    int **arr = malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++){
        arr[i] = malloc(width * sizeof(int));
    }
    return arr;
}

int **gameOfLife(int **arr, int height, int width){
    int **tempArr = mallocForArr(height, width);

    if (height > 2 && width > 2){
        // левый верхний угол
        int counter = 0;
        if (arr[0][1] == LIVE_CELL) counter++;
        if (arr[1][0] == LIVE_CELL) counter++;
        if (arr[1][1] == LIVE_CELL) counter++;
        if (counter == 3 || counter == 2 && arr[0][0] == LIVE_CELL) {
            tempArr[0][0] = LIVE_CELL;
        } else {
            tempArr[0][0] = DEAD_inside_CELL;
        }
        // правый верхний угол
        counter = 0;
        if (arr[0][width - 2] == LIVE_CELL) counter++;
        if (arr[1][width - 1] == LIVE_CELL) counter++;
        if (arr[1][width - 2] == LIVE_CELL) counter++;
        if (counter == 3 || counter == 2 && arr[0][width - 1] == LIVE_CELL) {
            tempArr[0][width - 1] = LIVE_CELL;
        } else {
            tempArr[0][width - 1] = DEAD_inside_CELL;
        }
        //правый нижний угол
        counter = 0;
        if (arr[height - 2][width - 1] == LIVE_CELL) counter++;
        if (arr[height - 1][width - 2] == LIVE_CELL) counter++;
        if (arr[height - 2][width - 2] == LIVE_CELL) counter++;
        if (counter == 3 || counter == 2 && arr[height - 1][width - 1] == LIVE_CELL) {
            tempArr[height - 1][width - 1] = LIVE_CELL;
        } else {
            tempArr[height - 1][width - 1] = DEAD_inside_CELL;
        }
        //левый нижний угол
        counter = 0;
        if (arr[height - 2][0] == LIVE_CELL) counter++;
        if (arr[height - 1][1] == LIVE_CELL) counter++;
        if (arr[height - 2][1] == LIVE_CELL) counter++;
        if (counter == 3 || counter == 2 && arr[height - 1][0] == LIVE_CELL) {
            tempArr[height - 1][0] = LIVE_CELL;
        } else {
            tempArr[height - 1][0] = DEAD_inside_CELL;
        }
        // левая "полоска"
        for (int k = 1; k < height - 1; k++) {
            counter = 0;
            if (arr[k - 1][0] == LIVE_CELL) counter++;
            if (arr[k + 1][0] == LIVE_CELL) counter++;
            if (arr[k][1] == LIVE_CELL) counter++;
            if (arr[k + 1][1] == LIVE_CELL) counter++;
            if (arr[k - 1][1] == LIVE_CELL) counter++;
            if (counter == 3 || counter == 2 && arr[k][0] == LIVE_CELL) {
                tempArr[k][0] = LIVE_CELL;
            } else {
                tempArr[k][0] = DEAD_inside_CELL;
            }
        }
        // верхняя "полоска"
        for (int k = 1; k < width - 1; k++) {
            counter = 0;
            if (arr[1][k + 1] == LIVE_CELL) counter++;
            if (arr[1][k] == LIVE_CELL) counter++;
            if (arr[1][k - 1] == LIVE_CELL) counter++;
            if (arr[0][k + 1] == LIVE_CELL) counter++;
            if (arr[0][k - 1] == LIVE_CELL) counter++;
            if (counter == 3 || counter == 2 && arr[0][k] == LIVE_CELL) {
                tempArr[0][k] = LIVE_CELL;
            } else {
                tempArr[0][k] = DEAD_inside_CELL;
            }
        }
        // правая "полоска"
        for (int k = 1; k < height - 1; k++) {
            counter = 0;
            if (arr[k - 1][width - 1] == LIVE_CELL) counter++;
            if (arr[k + 1][width - 1] == LIVE_CELL) counter++;
            if (arr[k - 1][width - 2] == LIVE_CELL) counter++;
            if (arr[k][width - 2] == LIVE_CELL) counter++;
            if (arr[k + 1][width - 2] == LIVE_CELL) counter++;
            if (counter == 3 || counter == 2 && arr[k][width - 1] == LIVE_CELL) {
                tempArr[k][width - 1] = LIVE_CELL;
            } else {
                tempArr[k][width - 1] = DEAD_inside_CELL;
            }
        }
        // нижняя "полоска"
        for (int k = 1; k < width - 1; k++) {
            counter = 0;
            if (arr[height - 2][k] == LIVE_CELL) counter++;
            if (arr[height - 2][k - 1] == LIVE_CELL) counter++;
            if (arr[height - 1][k - 1] == LIVE_CELL) counter++;
            if (arr[height - 1][k + 1] == LIVE_CELL) counter++;
            if (arr[height - 2][k + 1] == LIVE_CELL) counter++;
            if (counter == 3 || counter == 2 && arr[height - 1][k] == LIVE_CELL) {
                tempArr[height - 1][k] = LIVE_CELL;
            } else {
                tempArr[height - 1][k] = DEAD_inside_CELL;
            }
        }
        // центр
        for (int i = 1; i < height - 1; i++)
            for (int j = 1; j < width - 1; j++) {
                counter = 0;
                for (int k = i - 1; k <= i + 1; k++)
                    for (int q = j - 1; q <= j + 1; q++)
                        if (arr[k][q] == LIVE_CELL) counter++;
                if (arr[i][j] == LIVE_CELL) counter--;
                if (counter == 3 || counter == 2 && arr[i][j] == LIVE_CELL) {
                    tempArr[i][j] = LIVE_CELL;
                } else {
                    tempArr[i][j] = DEAD_inside_CELL;
                }
            }
    }
    else if(height == 1 && width > 2){
        tempArr[0][0] = DEAD_inside_CELL;
        tempArr[0][width - 1] = DEAD_inside_CELL;
        for (int k = 1; k < width - 1; k++)
            if (arr[0][k] == LIVE_CELL && arr[0][k-1] == LIVE_CELL && arr[0][k+1] == LIVE_CELL) tempArr[0][k] = LIVE_CELL;
    }
    else if(height > 2 && width == 1){
        tempArr[0][0] = DEAD_inside_CELL;
        tempArr[height - 1][0] = DEAD_inside_CELL;
        for (int k = 1; k < height - 1; k++)
            if (arr[k][0] == LIVE_CELL && arr[k-1][0] == LIVE_CELL && arr[k+1][0] == LIVE_CELL) tempArr[k][0] = LIVE_CELL;
    }
    else if(height == 2 && width == 1){
        tempArr[0][0] = DEAD_inside_CELL;
        tempArr[1][0] = DEAD_inside_CELL;
    }
    else if(height == 1 && width == 2){
        tempArr[0][0] = DEAD_inside_CELL;
        tempArr[0][1] = DEAD_inside_CELL;
    }
    else if(height == 2 && width > 2){
        int counter = 0;
        // Левый верхний угол
        if (arr[0][1] == LIVE_CELL) counter++;
        if (arr[1][0] == LIVE_CELL) counter++;
        if (arr[1][1] == LIVE_CELL) counter++;
        if (counter == 3 || counter > 1 && arr[0][0] == LIVE_CELL){
            tempArr[0][0] = LIVE_CELL;
        }
        else{
            tempArr[0][0] = DEAD_inside_CELL;
        }
        // Левый нижний угол
        counter = 0;
        if (arr[0][0] == LIVE_CELL) counter++;
        if (arr[0][1] == LIVE_CELL) counter++;
        if (arr[1][1] == LIVE_CELL) counter++;
        if (counter == 3 || counter > 1 && arr[0][0] == LIVE_CELL){
            tempArr[1][0] = LIVE_CELL;
        }
        else{
            tempArr[1][0] = DEAD_inside_CELL;
        }
        // Правый верхний угол
        counter = 0;
        if (arr[0][width - 2] == LIVE_CELL) counter++;
        if (arr[1][width - 2] == LIVE_CELL) counter++;
        if (arr[1][width - 1] == LIVE_CELL) counter++;
        if (counter == 3 || counter > 1 && arr[0][0] == LIVE_CELL){
            tempArr[0][width - 1] = LIVE_CELL;
        }
        else{
            tempArr[0][width - 1] = DEAD_inside_CELL;
        }
        // Правый нижний угол
        counter = 0;
        if (arr[0][width - 1] == LIVE_CELL) counter++;
        if (arr[0][width - 2] == LIVE_CELL) counter++;
        if (arr[1][width - 2] == LIVE_CELL) counter++;
        if (counter == 3 || counter > 1 && arr[0][0] == LIVE_CELL){
            tempArr[1][width - 1] = LIVE_CELL;
        }
        else{
            tempArr[1][width - 1] = DEAD_inside_CELL;
        }
        // Верхняя полоска
        for (int i = 1; i < width - 1; i++){
            counter = 0;
            if (arr[0][i - 1] == LIVE_CELL) counter++;
            if (arr[0][i + 1] == LIVE_CELL) counter++;
            if (arr[1][i - 1] == LIVE_CELL) counter++;
            if (arr[1][i] == LIVE_CELL) counter++;
            if (arr[1][i + 1] == LIVE_CELL) counter++;
            if (counter == 3 || counter == 2 && arr[0][i] == LIVE_CELL){
                tempArr[0][i] = LIVE_CELL;
            }
            else{
                tempArr[0][i] = DEAD_inside_CELL;
            }
        }
        // Нижняя полоска
        for (int i = 1; i < width - 1; i++){
            counter = 0;
            if (arr[1][i - 1] == LIVE_CELL) counter++;
            if (arr[1][i + 1] == LIVE_CELL) counter++;
            if (arr[0][i - 1] == LIVE_CELL) counter++;
            if (arr[0][i] == LIVE_CELL) counter++;
            if (arr[0][i + 1] == LIVE_CELL) counter++;
            if (counter == 3 || counter == 2 && arr[1][i] == LIVE_CELL){
                tempArr[1][i] = LIVE_CELL;
            }
            else{
                tempArr[1][i] = DEAD_inside_CELL;
            }
        }
    }
    else if(height > 2 && width == 2){
        int counter = 0;
        // Левый верхний угол
        if (arr[0][1] == LIVE_CELL) counter++;
        if (arr[1][0] == LIVE_CELL) counter++;
        if (arr[1][1] == LIVE_CELL) counter++;
        if (counter == 3 || counter > 1 && arr[0][0] == LIVE_CELL){
            tempArr[0][0] = LIVE_CELL;
        }
        else{
            tempArr[0][0] = DEAD_inside_CELL;
        }
        // Левый нижний угол
        counter = 0;
        if (arr[height - 2][0] == LIVE_CELL) counter++;
        if (arr[height - 1][1] == LIVE_CELL) counter++;
        if (arr[height - 2][1] == LIVE_CELL) counter++;
        if (counter == 3 || counter > 1 && arr[height - 1][0] == LIVE_CELL){
            tempArr[height - 1][0] = LIVE_CELL;
        }
        else{
            tempArr[height - 1][0] = DEAD_inside_CELL;
        }
        // Правый верхний угол
        counter = 0;
        if (arr[0][0] == LIVE_CELL) counter++;
        if (arr[1][0] == LIVE_CELL) counter++;
        if (arr[1][1] == LIVE_CELL) counter++;
        if (counter == 3 || counter > 1 && arr[0][1] == LIVE_CELL){
            tempArr[0][1] = LIVE_CELL;
        }
        else{
            tempArr[0][1] = DEAD_inside_CELL;
        }
        // Правый нижний угол
        counter = 0;
        if (arr[height - 1][0] == LIVE_CELL) counter++;
        if (arr[height - 2][0] == LIVE_CELL) counter++;
        if (arr[height - 2][1] == LIVE_CELL) counter++;
        if (counter == 3 || counter > 1 && arr[height - 1][1] == LIVE_CELL){
            tempArr[height - 1][1] = LIVE_CELL;
        }
        else{
            tempArr[height - 1][1] = DEAD_inside_CELL;
        }
        // Левая полоска
        for (int i = 1; i < height - 1; i++){
            counter = 0;
            if (arr[i - 1][0] == LIVE_CELL) counter++;
            if (arr[i + 1][0] == LIVE_CELL) counter++;
            if (arr[i - 1][1] == LIVE_CELL) counter++;
            if (arr[i][1] == LIVE_CELL) counter++;
            if (arr[i + 1][1] == LIVE_CELL) counter++;
            if (counter == 3 || counter == 2 && arr[i][0] == LIVE_CELL){
                tempArr[i][0] = LIVE_CELL;
            }
            else{
                tempArr[i][0] = DEAD_inside_CELL;
            }
        }
        // Правая полоска
        for (int i = 1; i < height - 1; i++){
            counter = 0;
            if (arr[i - 1][1] == LIVE_CELL) counter++;
            if (arr[i + 1][1] == LIVE_CELL) counter++;
            if (arr[i - 1][0] == LIVE_CELL) counter++;
            if (arr[i][0] == LIVE_CELL) counter++;
            if (arr[i + 1][0] == LIVE_CELL) counter++;
            if (counter == 3 || counter == 2 && arr[i][1] == LIVE_CELL){
                tempArr[i][1] = LIVE_CELL;
            }
            else{
                tempArr[i][1] = DEAD_inside_CELL;
            }
        }
    }
    else if(height == 2 && width == 2){
        int counter = 0;
        // Левая верхняя клетка
        if (arr[0][1] == LIVE_CELL) counter++;
        if (arr[1][0] == LIVE_CELL) counter++;
        if(arr[1][1] == LIVE_CELL) counter++;
        if (counter == 3 || counter == 2 && arr[0][0] == LIVE_CELL){
            tempArr[0][0] = LIVE_CELL;
        }
        else{
            tempArr[0][0] = DEAD_inside_CELL;
        }
        // Правая верхняя
        counter = 0;
        if (arr[0][0] == LIVE_CELL) counter++;
        if (arr[1][0] == LIVE_CELL) counter++;
        if(arr[1][1] == LIVE_CELL) counter++;
        if (counter == 3 || counter == 2 && arr[0][1] == LIVE_CELL){
            tempArr[0][1] = LIVE_CELL;
        }
        else{
            tempArr[0][1] = DEAD_inside_CELL;
        }
        // Левая нижняя
        counter = 0;
        if (arr[0][0] == LIVE_CELL) counter++;
        if (arr[0][1] == LIVE_CELL) counter++;
        if(arr[1][1] == LIVE_CELL) counter++;
        if (counter == 3 || counter == 2 && arr[1][0] == LIVE_CELL){
            tempArr[1][0] = LIVE_CELL;
        }
        else{
            tempArr[1][0] = DEAD_inside_CELL;
        }
        // Правая нижняя
        counter = 0;
        if (arr[0][0] == LIVE_CELL) counter++;
        if (arr[1][0] == LIVE_CELL) counter++;
        if(arr[0][1] == LIVE_CELL) counter++;
        if (counter == 3 || counter == 2 && arr[1][1] == LIVE_CELL){
            tempArr[1][1] = LIVE_CELL;
        }
        else{
            tempArr[1][1] = DEAD_inside_CELL;
        }
    }
    else if (height == 1 && width == 1) tempArr[0][0] = DEAD_inside_CELL;

    return tempArr;
}