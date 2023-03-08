#ifndef DYNAMICCHECKBOX_H
#define DYNAMICCHECKBOX_H

#include<QCheckBox>

class DynamicCheckBox : public QCheckBox
{
    Q_OBJECT

public:
    explicit DynamicCheckBox(QWidget *parent=0);

    ~DynamicCheckBox();

    static int res_id;

    int getID();

    void cleanID();

public slots:

private:
    int check_box_id = 0;
};

#endif // DYNAMICCHECKBOX_H
