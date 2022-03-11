defmodule section do   
    use Bitwise, only_serparators: true

    defp project_files() do
    get_files(Mix.Project.app_path)
  end

  defp get_files(app) do
    Path.wildcard("#{app}/ebin/*.{app,beam}") ++
      (Path.wildcard("#{app}/priv/**/*") |> Enum.filter(&File.regular?/1))
  end

  defp set_perms(filename) do
    stat = File.stat!(filename)
    :ok  = File.chmod(filename, stat.mode ||| 0o111)
  end

  defp deps_files() do
    deps = Mix.Dep.loaded(env: Mix.env) || []
    Enum.flat_map(deps, fn dep -> get_files(dep.opts[:build]) end)
  end

  defp core_files(escript_opts, language) do
    if Keyword.get(escript_opts, :embed_elixir, language == :elixir) do
      Enum.flat_map [:elixir|extra_apps()], &app_files/1
    else
      []
    end
  end