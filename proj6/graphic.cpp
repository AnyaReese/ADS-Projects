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
    int flag; //
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

                x2 = x1 + wide[j].width;
                y1 += wide[j].height;
                wide.erase(wide.begin() + j);
                j--;
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

void MainWindow::packNarrow2(int x1, int y1, int x2, int y2) {
    if (narrow.size()) {
        // pack first narrow rectangle that
        // fits height-wise and width-wise
        int i;
        for (i = 0; i < narrow.size(); i++) {
            if (x2 - x1 >= narrow[i].width
                && y2 - y1 >= narrow[i].height)
                break;
        }
        // if running out of narrow, then return
        if(i == narrow.size())
            return;
        narrow[i].x = x1;
        narrow[i].y = y1;
        list.push_back(narrow[i]);
        int deltax = narrow[i].width;
        int deltay = narrow[i].height;
        narrow.erase(narrow.begin() + i);
        // call itself to fill the remaining space on the
        // top of the packed rectangle and on the right of
        // the packed rectangle
        packNarrow2(x1, y1 + deltay, x1 + deltax, y2);
        packNarrow2(x1 + deltax, y1, x2, y2);
    }
        // if running out of narrow rectangle,
        // we can fill the remaining space with wide rectangles
        // in SAS_advanced algorithm
    else if (algoNum == 3) {
        packWide2(x1, y1, x2, y2);
    }
}

// packWide2 subroutine only for SAS_advanced algorithm
//
void MainWindow::packWide2(int x1, int y1, int x2, int y2) {
    if (wide.size() && x2 - x1 >= wide.back().width) {
        int j;
        // choose the first rectangle from wide
        // that can fit both wide-wise and height-wise
        for (j = 0; j < wide.size(); j++)
            if (x2 - x1 >= wide[j].width)
                break;
        if (j == wide.size())
            return;

        int start;
        for (start = j; start < wide.size(); start++) {
            if (y2 - y1 >= wide[start].height)
                break;
        }
        if (start == wide.size())
            return;

        j = start;
        int x3 = wide[j].width + x1;
        int y3 = y1;
        // while existing rectangles that can fit height-wise
        while (j != wide.size() && y2 - y1 >= wide[j].height) {
            wide[j].x = x1;
            wide[j].y = y1;
            y1 += wide[j].height;
            list.push_back(wide[j]);
            wide.erase(wide.begin() + j);
        }
        // recursively call packWide2 for fill the
        // remaining space that are on the right-hand side
        packWide2(x3, y3, x2, y2);
    }
    return;
}


