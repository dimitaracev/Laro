#include <iostream>
#include <cstring>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>

extern "C"
{
    #include "lex.yy.h"
    #include "y.tab.h"
    #include "src/ast.h" 
    #include "src/generate.h"
    #include "src/semantic.h"
    extern FILE* yyin;
    extern ast_node* root;
    extern code* mips_code;
    extern declared_functions* dfs;
    extern int current_label_count;
    extern int current_register_count;
    extern int current_temporary_register_count;
}

class App : public QApplication
{
    private:
    QMainWindow *window;
    QPushButton *run;
    QTextEdit *source_code;
    QTextEdit *compiled_code;
    QTextEdit *err_code;
    public:
    App(int argc, char* argv[]) : QApplication(argc, argv)
    {
        window = new QMainWindow(NULL);
        run = new QPushButton(window);
        source_code = new QTextEdit(window);
        compiled_code = new QTextEdit(window);
        err_code = new QTextEdit(window);
        
        run->setText("Run");
        run->setGeometry(300, 0, 70, 70);

        source_code->setGeometry(0, 0, 300, 300);
        
        compiled_code->setGeometry(370, 0, 300, 300);
        compiled_code->setReadOnly(true);

        err_code->setGeometry(0, 350, 600, 300);
        err_code->setTextColor(QColor(255, 0, 0));
        err_code->setReadOnly(true);

        connect(run, &QPushButton::clicked, [=](){
            current_label_count = 0;
            current_register_count = 0;
            current_temporary_register_count = 0;

            if(root != NULL)
                clear_nodes(root);
            
            root = create_ast_node();

            std::string l = source_code->toPlainText().toStdString();
            yy_scan_bytes(l.c_str(), l.length());
            yyparse();

            if(mips_code != NULL)
               clear_code(mips_code);

            if(error_code != NULL)
              clear_code(error_code);

            mips_code = new_code();
            error_code = new_code();
            
            if(dfs != NULL)
                clear_df(dfs);

            dfs = create_df();

            semantic(root);

            if(error_code->characters == 0)
            {
                generate_code(root);
                compiled_code->setText(mips_code->inst_code);
                err_code->setText("");
            }
            else {
                compiled_code->setText("");
                err_code->setText(error_code->inst_code);
            }
        });
        window->setWindowTitle("Laro Compiler");
        window->show();
    }

    ~App()
    {
        delete run;
        delete source_code;
        delete err_code;
        delete window;
    }
};






int main(int argc, char* argv[])
{
    App* app = new App(argc, argv);
    return app->exec();
}
