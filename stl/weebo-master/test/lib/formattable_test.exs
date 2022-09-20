defmodule Weebo.FormattableTest do
  use ExUnit.Case
  import Weebo.Formattable, only: [format: 1]

  test "for: BitString" do
    assert format("hello") == {:string, ["hello"]}
  end

  test "for: Integer" do
    assert format(40) == {:int, ["40"]}
  end

  test "for: Float" do
    assert format(-0.5) == {:double, ["-0.5"]}
  end

  test "for: Boolean" do
    assert format(true) == {:boolean, ["1"]}
    assert format(false) == {:boolean, ["0"]}
  end

  test "for: Map" do
    assert format(%{hello: "world", ready: true}) == {:struct, [{:member, [{:name, ["hello"]}, {:value, [{:string, ["world"]}]}]}, {:member, [{:name, ["ready"]}, {:value, [{:boolean, ["1"]}]}]}]}
  end

  test "for: nil" do
    assert format(nil) == {:nil, []}
  end

  test "for: DateTime" do
    time = :calendar.universal_time
    assert format(time) == {:"dateTime.iso8601", [:iso8601.format(time)]}
  end

  test "for: List" do
    assert format([1, "hello", false]) == {:array, [{:data, [{:value, [{:int, ["1"]}]}, {:value, [{:string, ["hello"]}]}, {:value, [{:boolean, ["0"]}]}]}]}
  end
end
