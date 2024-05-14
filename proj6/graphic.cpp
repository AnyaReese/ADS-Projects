void MainWindow::SAS() {
    // following are erasing the previous information
    // in the containers that are to be used
    narrow.clear();
    wide.clear();
    list.clear();

    // partition the original rectangles into two sorts:
    // narrow and wide
    for each (rectangle rec in original){
        if (rec.height > rec.width)
            narrow.push_back(rec);
        else
            wide.push_back(rec);
    }

    // if we use the SAS algorithm,
    // we only order two lists of rectangles in one dimension
    if (algoNum == 2) {
        sort(wide.begin(), wide.end(), greater_w);
        sort(narrow.begin(), narrow.end(), greater_h);
    }

    // following are some useful local variables when processing
    int currentX1, currentY1;
    int flag;
    int actualHeight = 0;	// also it's the height we have achieved

    while ( narrow.size() || wide.size() ) {
        // pick up the tallest rectangle between the firt rectangle
        // in the narrow list and that in the wide lists

        // if both narrow and wide is not empty,
        if ( narrow.size() && wide.size() ) {
            if (narrow[0].height > wide[0].height) {
                // pack the narrow
                narrow[0].x = 0;
                narrow[0].y = actualHeight;
                currentX1 = narrow[0].width;
                currentY1 = actualHeight;
                actualHeight += narrow[0].height;
                list.push_back(narrow[0]);
                narrow.erase(narrow.begin());
                flag = 0;
            }
            else {
                // pack the wide
                wide[0].x = 0;
                wide[0].y = actualHeight;
                currentX1 = wide[0].width;
                currentY1 = actualHeight;
                actualHeight += wide[0].height;
                list.push_back(wide[0]);
                wide.erase(wide.begin());
                flag = 1;
            }
        }
            // if wide is empty, the tallest must come from narrow
        else if ( narrow.size() ) {
            // pack the narrow
            narrow[0].x = 0;
            narrow[0].y = actualHeight;
            currentX1 = narrow[0].width;
            currentY1 = actualHeight;
            actualHeight += narrow[0].height;
            list.push_back(narrow[0]);
            narrow.erase(narrow.begin());
            flag = 0;
        }
            // if narrow is empty, the tallest must come from wide
        else {
            // pack the wide
            wide[0].x = 0;
            wide[0].y = actualHeight;
            currentX1 = wide[0].width;
            currentY1 = actualHeight;
            actualHeight += wide[0].height;
            list.push_back(wide[0]);
            wide.erase(wide.begin());
            flag = 1;
        }

        // if tallest rectangle is from the narrow list
        // we use packWide to fill the remaining space
        if (flag == 0)
            packWide(currentX1, currentY1, fixedWidth, actualHeight);

            // if tallest rectangle is from the wide list
            // we use packNarrow to fill the remaing space
        else {
            packNarrow(currentX1, currentY1, fixedWidth, actualHeight);
        }

    }

    // everything has been done, and now we are gonna to
    // show our result, including the graphic result,
    // the height we achieved and the calculated approximation ratio
    done = true;
    QString tempStr;
    tempStr.setNum(actualHeight);
    RLine1->setText(tempStr);
    tempStr.setNum(double(actualHeight) / optimalHeight);
    RLine2->setText(tempStr);
}

// packWide subroutine
// it may call packNarrow again and again
// it works in attempt to fill the remaining space
// when the tallest rectangles in one level is from narrow
void MainWindow::packWide(int x1, int y1, int x2, int y2) {
    // if the basic condition is satisfied, i.e.
    // wide is not empty and the first rectangle in wide
    // can be packed. Otherwise we can do nothing
    if (wide.size() && x2 - x1 >= wide[0].width) {
        for (int j = 0; j < wide.size(); j++) {
            // if rectangle fits wide-wise and height-wise
            if (y2 - y1 >= wide[j].height) {
                wide[j].x = x1;
                wide[j].y = y1;
                list.push_back(wide[j]);

                // if rectangles of unequal widths are stacked(SAS)
                // or if rectangle just packed has created a space
                // on the right of it (SAS_advaneced)
                // we use packNarrow to reduce the redundant space
                // further
                if (x2 - (x1 + wide[j].width) > 0)
                    if(x2 != fixedWidth && algoNum == 2)
                        packNarrow(x1 + wide[j].width, y1, x2, y2);
                    else if(algoNum == 3){
                        packNarrow2(x1 + wide[j].width, y1, x2, y2);
                    }
                x2 = x1 + wide[j].width;
                y1 += wide[j].height;
                wide.erase(wide.begin() + j);
                j--;
            }
                // In SAS_advanced algorithm, if wide are running out of
                // rectangles, we can pack the narrow rectangles in the
                // remaining space
            else if (algoNum == 3) {
                packNarrow(x1, y1, x2, y2);
            }
        }
    }
    return;
}

// packNarrow subroutine, only for SAS algorithm
// it doesn't work in the perfect way
void MainWindow::packNarrow(int x1, int y1, int x2, int y2) {
    // when the narrow is not running out of rectangle
    if (narrow.size()) {
        int baseWidth;
        int currentX1 = x1;	// the coordinate of the starting point
        int currentY1 = y1;	// that we may pack a rectangle in
        // while the rectangle can fit wide-wise
        while (narrow.size() && x2 - currentX1 >= narrow[0].width) {
            baseWidth = narrow[0].width;
            //while the rectangle can fit height-wise
            while (narrow.size() && y2 - currentY1 >= narrow[0].height
                   && narrow[0].width <= baseWidth) {
                narrow[0].x = currentX1;
                narrow[0].y = currentY1;
                list.push_back(narrow[0]);
                // turn to next vertical space
                currentY1 += narrow[0].height;
                narrow.erase(narrow.begin());
            }
            // turn to next horizontal space
            currentX1 += baseWidth;
            currentY1 = y1;
        }
    }
    return;
}