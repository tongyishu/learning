

#### GLIB

glib 本质上是使用面向过程的语言（c）编译面向对象的代码。
gobject使用两种类型来表示一个"类"："对象结构体"和类结构体"。这两种结构体的实例为分别称为"对象实例"和"类实例"。这个"类"的实例叫做"对象"。

```c++
g_object_get_property()           // 获取对象实例的属性
g_object_get_property()           // 设置对象实例的属性
g_object_class_install_property   // 设置类实例的属性
```






#### G_DEFINE_TYPE 

- G_DEFINE_TYPE宏展开后生成xxx_get_type函数，该函数返回要定义类的ID，此ID全局唯一。

```c
// 示例代码
// G_DEFINE_TYPE(TN, t_n, T_P)
// @TN: 自定义类型, 驼峰格式。
// @t_n: 自定义类型, 小写字母加下划线格式。
// @T_P: 父类型。

// 预处理：gcc -E ./test.c -lgobject-2.0 -lglib-2.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include > ./test.out
// 编译：gcc -g -o ./test.out ./test.c -lgobject-2.0 -lglib-2.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include
// 运行：./test.out


#include <glib-object.h>

typedef struct _Tongyishu Tongyishu;
typedef struct _TongyishuClass TongyishuClass;

struct _Tongyishu
{
    GObject parent;
    gint type;
};

struct _TongyishuClass
{
    GObjectClass parent_class;
};

G_DEFINE_TYPE(Tongyishu, tongyishu, G_TYPE_OBJECT)

static void tongyishu_init(Tongyishu *self) {}
static void tongyishu_class_init(TongyishuClass *klass) {}

int main()
{
    Tongyishu *tongyishu = NULL;
    tongyishu = g_object_new(tongyishu_get_type(), NULL);
    g_print("+++ tongyishu_get_type() = %d\n", tongyishu_get_type());
    return 0;
}
```

- G_DEFINE_TYPE(Tongyishu, tongyishu, G_TYPE_OBJECT)宏展开

```c++
/* G_DEFINE_TYPE(Tongyishu, tongyishu, G_TYPE_OBJECT) BEGIN */
static void tongyishu_init(Tongyishu *self);
static void tongyishu_class_init(TongyishuClass *klass);
static gpointer tongyishu_parent_class = ((void *)0);
static gint Tongyishu_private_offset;
static void tongyishu_class_intern_init(gpointer klass)
{
    tongyishu_parent_class = g_type_class_peek_parent(klass);
    if (Tongyishu_private_offset != 0)
        g_type_class_adjust_private_offset(klass, &Tongyishu_private_offset);
    tongyishu_class_init((TongyishuClass *)klass);
}
__attribute__((__unused__)) static __inline gpointer tongyishu_get_instance_private(Tongyishu *self) { return (((gpointer)((guint8 *)(self) + (glong)(Tongyishu_private_offset)))); }
GType tongyishu_get_type(void)
{
    static volatile gsize g_define_type_id__volatile = 0;
    if ((__extension__(
            {
                typedef char _GStaticAssertCompileTimeAssertion_0[(sizeof *(&g_define_type_id__volatile) == sizeof(gpointer)) ? 1 : -1] __attribute__((__unused__));
                (void)(0 ? (gpointer) * (&g_define_type_id__volatile) : 0);
                (!(__extension__(
                     {
                         typedef char _GStaticAssertCompileTimeAssertion_1[(sizeof *(&g_define_type_id__volatile) == sizeof(gpointer)) ? 1 : -1] __attribute__((__unused__));
                         (gpointer) __atomic_load_8((&g_define_type_id__volatile), 5);
                     })) &&
                 g_once_init_enter(&g_define_type_id__volatile));
            })))
    {
        GType g_define_type_id = g_type_register_static_simple(((GType)((20) << (2))), g_intern_static_string("Tongyishu"), sizeof(TongyishuClass), (GClassInitFunc)(void (*)(void))tongyishu_class_intern_init, sizeof(Tongyishu), (GInstanceInitFunc)(void (*)(void))tongyishu_init, (GTypeFlags)0);
        {{{};
    }
}
(__extension__(
    {
        typedef char _GStaticAssertCompileTimeAssertion_2[(sizeof *(&g_define_type_id__volatile) == sizeof(gpointer)) ? 1 : -1] __attribute__((__unused__));
        (void)(0 ? *(&g_define_type_id__volatile) = (g_define_type_id) : 0);
        g_once_init_leave((&g_define_type_id__volatile), (gsize)(g_define_type_id));
    }));
}
return g_define_type_id__volatile;
}
/* G_DEFINE_TYPE(Tongyishu, tongyishu, G_TYPE_OBJECT) END */

```

