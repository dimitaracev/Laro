#include <iostream>
#include <cstring>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

extern "C"
{
#include "lex.yy.h"
#include "y.tab.h"
#include "src/ast.h"
#include "src/generate.h"
#include "src/semantic.h"
    extern ast_node *root;
    code *mips_code;
    code *error_code;
    declared_functions *dfs;
    extern int current_label_count;
    extern int current_register_count;
    extern int current_temporary_register_count;
    extern int stack_pointer;
}

class App : public QApplication
{
private:
    QMainWindow *window;
    QPushButton *run;
    QTextEdit *source_code;
    QTextEdit *compiled_code;
    QTextEdit *err_code;
    QLabel *label_source_code;
    QLabel *label_compiled_code;
    QLabel *label_err_code;

public:
    App(int argc, char *argv[]) : QApplication(argc, argv)
    {
        window = new QMainWindow(NULL);
        run = new QPushButton(window);
        source_code = new QTextEdit(window);
        compiled_code = new QTextEdit(window);
        err_code = new QTextEdit(window);

        label_source_code = new QLabel(window);
        label_source_code->setText("Source Code");
        label_source_code->setGeometry(150, 0, 100, 50);

        label_compiled_code = new QLabel(window);
        label_compiled_code->setText("Compiled Code");
        label_compiled_code->setGeometry(550, 0, 100, 50);

        label_err_code = new QLabel(window);
        label_err_code->setText("Error Code");
        label_err_code->setGeometry(50, 350, 100, 50);

        run->setText("Run");
        run->setGeometry(350, 150, 100, 70);

        source_code->setGeometry(50, 50, 300, 300);

        compiled_code->setGeometry(450, 50, 300, 300);
        compiled_code->setReadOnly(true);

        err_code->setGeometry(0, 400, 800, 300);
        err_code->setTextColor(QColor(255, 0, 0));
        err_code->setReadOnly(true);

        connect(run, &QPushButton::clicked, [=]() {
            current_label_count = 0;
            current_register_count = 0;
            current_temporary_register_count = 0;
            stack_pointer = 0;

            if (root != NULL)
                clear_nodes(root);

            root = create_ast_node();

            std::string s_code = source_code->toPlainText().toStdString();
            yy_scan_bytes(s_code.c_str(), s_code.length());
            yyparse();

            if (mips_code != NULL)
                clear_code(mips_code);

            if (error_code != NULL)
                clear_code(error_code);

            mips_code = new_code();
            error_code = new_code();

            if (dfs != NULL)
                clear_df(dfs);

            dfs = create_df();

            semantic(error_code, dfs, root);

            if (error_code->characters == 0)
            {
                generate_code(mips_code, root);
                compiled_code->setText(mips_code->inst_code);
                err_code->setText("");
            }
            else
            {
                compiled_code->setText("");
                err_code->setText(error_code->inst_code);
            }
        });
        window->setFixedSize(800, 800);
        window->setWindowTitle("Laro Compiler");
        window->show();
    }

    ~App()
    {
        clear_nodes(root);
        clear_df(dfs);
        clear_code(mips_code);
        clear_code(error_code);
        delete label_source_code;
        delete label_compiled_code;
        delete label_err_code;
        delete run;
        delete source_code;
        delete err_code;
        delete window;
    }
};

int main(int argc, char *argv[])
{
    App *app = new App(argc, argv);
    return app->exec();
}
