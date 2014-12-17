#include "pch.h"

namespace NativeScript {
namespace Tests {

TEST_CLASS(ClassesTests) {
public:
    TEST_METHOD(EmptyClass) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.EmptyClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        Assert::IsTrue(declaration->name() == L"EmptyClass");
        Assert::IsTrue(declaration->fullName() == name);
        Assert::IsTrue(declaration->baseFullName() == L"System.Object");
    }

    TEST_METHOD(Methods) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.SimpleClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        IteratorRange<ClassDeclaration::MethodIterator> methods{declaration->methods()};
        ClassDeclaration::MethodIterator it{methods.begin()};

        MethodDeclaration staticMethod = *it;
        Assert::IsTrue(staticMethod.name() == L"StaticMethod");
        Assert::IsTrue(staticMethod.fullName() == L"NativeScript.Tests.Fixtures.SimpleClass.StaticMethod");
        Assert::IsTrue(staticMethod.isStatic() == true);

        ++it;
        MethodDeclaration instanceMethod = *it;
        Assert::IsTrue(instanceMethod.name() == L"InstanceMethod");
        Assert::IsTrue(instanceMethod.fullName() == L"NativeScript.Tests.Fixtures.SimpleClass.InstanceMethod");
        Assert::IsTrue(instanceMethod.isStatic() == false);

        ++it;
        Assert::IsTrue(it == methods.end());
    }

    TEST_METHOD(Properties) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.SimpleClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        IteratorRange<ClassDeclaration::PropertyIterator> properties{declaration->properties()};
        ClassDeclaration::PropertyIterator it{properties.begin()};

        PropertyDeclaration staticReadonlyProperty = *it;
        Assert::IsTrue(staticReadonlyProperty.name() == L"StaticReadonlyProperty");
        Assert::IsTrue(staticReadonlyProperty.fullName() == L"NativeScript.Tests.Fixtures.SimpleClass.StaticReadonlyProperty");
        Assert::IsTrue(staticReadonlyProperty.getter()->name() == L"get_StaticReadonlyProperty");
        Assert::IsTrue(staticReadonlyProperty.setter() == nullptr);
        Assert::IsTrue(staticReadonlyProperty.isStatic() == true);

        ++it;
        PropertyDeclaration instanceReadonlyProperty = *it;
        Assert::IsTrue(instanceReadonlyProperty.name() == L"InstanceReadonlyProperty");
        Assert::IsTrue(instanceReadonlyProperty.fullName() == L"NativeScript.Tests.Fixtures.SimpleClass.InstanceReadonlyProperty");
        Assert::IsTrue(instanceReadonlyProperty.getter()->name() == L"get_InstanceReadonlyProperty");
        Assert::IsTrue(instanceReadonlyProperty.setter() == nullptr);
        Assert::IsTrue(instanceReadonlyProperty.isStatic() == false);

        ++it;
        Assert::IsTrue(it == properties.end());
    }

    // TEST_METHOD(StaticAttribute) {
    //     MetadataReader metadataReader;
    //
    //     shared_ptr<ClassDeclaration> emptyClass{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.EmptyClass"))};
    //     Assert::IsTrue(emptyClass->staticInterface() == nullptr);
    //
    //     shared_ptr<ClassDeclaration> simpleClass{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.SimpleClass"))};
    //     Assert::IsTrue(simpleClass->staticInterface()->fullName() == L"NativeScript.Tests.Fixtures.ISimpleClassStatic");
    //     Assert::IsTrue(simpleClass->staticInterface()->id() == IID{0xF07E1852, 0xC1F3, 0x5BE8,{0x5A, 0x6F, 0x6A, 0xC5, 0x36, 0xDE, 0x8E, 0xEB}});
    // }

    TEST_METHOD(DeclaringInterfaceForInstanceInitializers) {
        MetadataReader metadataReader;

        shared_ptr<ClassDeclaration> activatableClass{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.ActivatableClass"))};

        vector<shared_ptr<MethodDeclaration>> initializers{activatableClass->findMethodsWithName(COR_CTOR_METHOD_NAME_W)};
        Assert::IsTrue(initializers.size() == 3);

        shared_ptr<MethodDeclaration> initializer0{initializers[0]};
        size_t index0{0};
        unique_ptr<InterfaceDeclaration> factory0{ClassDeclaration::declaringInterfaceForMethod(*initializer0.get(), &index0)};
        Assert::IsTrue(initializer0->numberOfArguments() == 0);
        Assert::IsTrue(factory0 == nullptr);
        Assert::IsTrue(index0 == SIZE_T_MAX);

        shared_ptr<MethodDeclaration> initializer1{initializers[1]};
        size_t index1{0};
        unique_ptr<InterfaceDeclaration> factory1{ClassDeclaration::declaringInterfaceForMethod(*initializer1.get(), &index1)};
        Assert::IsTrue(initializer1->numberOfArguments() == 1);
        Assert::IsTrue(factory1->id() == IID{0XE1C8B4D2, 0XCA7C, 0X5558,{0X7B, 0X6C, 0XAB, 0XC7, 0X47, 0XB8, 0X71, 0X3B}});
        Assert::IsTrue(index1 == 0);

        shared_ptr<MethodDeclaration> initializer2{initializers[2]};
        size_t index2{0};
        unique_ptr<InterfaceDeclaration> factory2{ClassDeclaration::declaringInterfaceForMethod(*initializer2.get(), &index2)};
        Assert::IsTrue(initializer2->numberOfArguments() == 2);
        Assert::IsTrue(factory2->id() == IID{0XE1C8B4D2, 0XCA7C, 0X5558,{0X7B, 0X6C, 0XAB, 0XC7, 0X47, 0XB8, 0X71, 0X3B}});
        Assert::IsTrue(index2 == 1);
    }
};

}
}
