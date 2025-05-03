from pydantic import BaseModel, Field


class ParamEnum(BaseModel):
    value: str = Field(description="列挙値")
    description: str = Field(default="", description="列挙値の説明")


class FunctionParam(BaseModel):
    name: str = Field(description="引数名")
    type_: str = Field(
        description="引数の型", validation_alias="type", serialization_alias="type"
    )
    description: str = Field(default="", description="引数の説明")
    optional: bool = Field(
        default=False, description="引数を省略可能 (nilを受け付ける) 場合はtrueにする"
    )
    enum: list[ParamEnum] | None = Field(
        default=None, description="引数として受け付ける値の列挙"
    )


class FunctionReturn(BaseModel):
    type_: str = Field(
        description="戻り値の型", validation_alias="type", serialization_alias="type"
    )
    description: str = Field(default="", description="戻り値の説明")


class Function(BaseModel):
    name: str = Field(description="関数名")
    description: str = Field(default="", description="関数の説明")
    params: list[FunctionParam] = Field(default=[], description="関数の引数")
    returns: list[FunctionReturn] = Field(default=[], description="関数の戻り値")


class Class(BaseModel):
    name: str = Field(description="クラス名")
    description: str = Field(default="", description="クラスの説明")
    staticMethods: list[Function] = Field(default=[], description="静的メソッド")
    methods: list[Function] = Field(default=[], description="インスタンスメソッド")


class Module(BaseModel):
    name: str = Field(
        description="モジュール名",
        validation_alias="module",
        serialization_alias="module",
    )
    functions: list[Function] = Field(default=[], description="モジュールメソッド")
    classes: list[Class] = Field(default=[], description="モジュール提供クラス")
