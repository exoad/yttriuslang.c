//https://latex.codecogs.com/png.latex?
const { MessageEmbed } = require('discord.js');
const config = require('../../configs/config.json');
const chnl = require('../../configs/chnl.json');
const resource = require('../../configs/resource.json');
const superagent = require('superagent');
const colors = require('../../configs/colors.json');
module.exports = {
    config: {
        name: `tex`,
        category: '',
        description: '',
        aliases: [`latex`]
    },
    run: async (bot, message, args) => {
    try {
        let equ = args.slice(0).join(' ');

        if(!equ || equ == undefined || equ == null) {
            const embed = new MessageEmbed()
            .setTitle("Oops, looks like you are missing some parameters!")
            .addField("Command Requirements", "The user, you, should have some knowledge of the language of `TeX` and/or `LaTeX`")
            .addField("Usage", "```"+config.prefix+"tex {formula_equation}```")
            .addField("{formula_equation}", "Input all necessary values here. Your equation, etc.")
            .addField("Example Usage", "```"+config.prefix+"tex \\[ \\lim_{x \\to +\\infty} \\frac{3x^2 +7x^3}{x^2 +5x^4} = 3.\\] ```")
            .setFooter("Have questions? Join the support server!")
            .setThumbnail(resource.aw_snap)

            message.channel.send("**This is still an experimental feature!!!**")
            message.channel.send({embed})
        } else {
            message.channel.send("**Request:** "+message.author.username+message.author.discriminator, { files: [{ attachment: `https://latex.codecogs.com/png.latex?\\inline&space;\\dpi{300}&space;\\bg_white&space;\\huge&space;${equ}`, name: "final.png" }] }).then(m => {
                const embed = new MessageEmbed()
                .setTitle("Debug Info")
                .addField(":inbox_tray: Input", "```latex\n"+equ+"```")
                .addField("Output Errors", "NaN")
                message.channel.send({embed}).then(m => {
                    m.delete({ timeout: 5000 })
                });
            })
		bot.channels.cache.get('839269407439978557').send(`> **<long> messageAuthor(id[] pack)**: ${message.author.id}\n> **(vector <long double> ar_field)**: ${equ}\n> **parse(guild.id)**: ${message.guild.id}`);
        }
    } catch (err) {
        bot.channels.cache.get(chnl.errors).send(err.stack)
        const embed = new MessageEmbed()
        .setTitle("Whoops, looks like something went wrong!")
        .setThumbnail(resource.aw_snap)
        .setDescription("Use `"+config.prefix+"help` for a list of avaliable commands or use `"+config.prefix+"support` to join the support server!")
        .setFooter("Still facing issues? Join the support server!")
        message.channel.send({ embed }).then(m => {
            m.delete({ timeout: 5000 })
        });
    }
  }
}
