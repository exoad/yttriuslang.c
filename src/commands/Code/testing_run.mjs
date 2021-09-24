import { Engines } from "panlang";


const chnl = require("../../../configs/chnl.json");
module.exports = {
  config: {
    name: `testing_code_run`,
    category: "",
    description: "",
    aliases: ["t_c_r"],
  },
  run: async (bot, message, args) => {
    try {
      let lan = args[0]; 
      message.channel.send(lan);
      if(lan == "sh") {
        const { sh, bash, csh, dash, ksh, tcsh, zsh } = Engines.shell;
        let code = args.slice(1).join(" ");
        message.channel.send(code);
        bash(code)
          .then((res) => message.channel.send("```bash\n" + res.output + "```"))
          .catch((res) => message.channel.send(res.error));
      }
      
    } catch (err) {
      console.error(err);
      bot.channels.cache.get(chnl["pre-bed-errors"]).send(err);
    }
  },
};
